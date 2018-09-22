#include <external/etl/array.h>
#include <src/telecomms/fec/rs8.h>
#include <string.h>
#include <algorithm>

void Rs8::Encode(uint8_t data[], uint8_t parity[]) {
    // TODO(dingbenjamin): Assert size of data and parity
    assert(kNn - kNRoots > 0);

    memset(parity, 0, kNRoots * sizeof(uint8_t));
    uint32_t i;

    for (i = 0; i < kNn - kNRoots; i++) {
        uint8_t feedback = index_of[data[i] ^ parity[0]];
        if (feedback != kA0) {
            // feedback term is non-zero
            uint32_t j;
            for (j = 1; j < kNRoots; j++) {
                parity[j] ^= alpha_to[(feedback + gen_poly[kNRoots - j]) % kNn];
            }
        }

        // Shift the parity array down by one byte (0 <- 1)
        memmove(&(parity[0]), &(parity[1]), sizeof(uint8_t) * (kNRoots - 1));
        if (feedback != kA0) {
            parity[kNRoots - 1] = alpha_to[(feedback + gen_poly[0]) % kNn];
        } else {
            parity[kNRoots - 1] = 0;
        }
    }
}

int32_t Rs8::Decode(uint8_t data[], int32_t eras_pos[], int32_t no_eras) {
    int32_t deg_lambda;
    int32_t el;
    int32_t deg_omega;
    int32_t i, j, r, k;

    uint8_t u, q, tmp, num1, num2, den, discr_r;
    // lambda: Err + Eras Locator poly and syndrome poly
    uint8_t lambda[kNRoots + 1], s[kNRoots];
    uint8_t b[kNRoots + 1], t[kNRoots + 1], omega[kNRoots + 1];
    uint8_t root[kNRoots], reg[kNRoots + 1], loc[kNRoots];

    int32_t syn_error, count;

    // Form the syndromes; i.e., evaluate data(x) at roots of g(x)
    for (i = 0; i < kNRoots; i++) {
        s[i] = data[0];
    }

    for (j = 1; j < kNn; j++) {
        for (i = 0; i < kNRoots; i++) {
            if (s[i] == 0) {
                s[i] = data[j];
            } else {
                s[i] = data[j] ^
                       alpha_to[(index_of[s[i]] + (kFcr + i) * kPrim) % kNn];
            }
        }
    }

    // Convert syndromes to index form, checking for nonzero condition
    syn_error = 0;
    for (i = 0; i < kNRoots; i++) {
        syn_error |= s[i];
        s[i] = index_of[s[i]];
    }

    if (!syn_error) {
        // if syndrome is zero, data[] is a codeword and there are no
        // errors to correct. So return data[] unmodified
        //
        count = 0;
    } else {
        memset(&(lambda[1]), 0, kNRoots * sizeof(lambda[0]));
        lambda[0] = 1;

        if (no_eras > 0) {
            // Init lambda to be the erasure locator polynomial
            lambda[1] = alpha_to[(kPrim * (kNn - 1 - eras_pos[0])) % kNn];
            for (i = 1; i < no_eras; i++) {
                u = (kPrim * (kNn - 1 - eras_pos[i])) % kNn;
                for (j = i + 1; j > 0; j--) {
                    tmp = index_of[lambda[j - 1]];
                    if (tmp != kA0) {
                        lambda[j] ^= alpha_to[(u + tmp) % kNn];
                    }
                }
            }
        }

        for (i = 0; i < kNRoots + 1; i++) {
            b[i] = index_of[lambda[i]];
        }

        //
        // Begin Berlekamp-Massey algorithm to determine error+erasure
        // locator polynomial
        //

        // r is the step number
        r = no_eras;
        el = no_eras;
        while (++r <= kNRoots) {
            // Compute discrepancy at the r-th step in poly-form
            discr_r = 0;
            for (i = 0; i < r; i++) {
                if ((lambda[i] != 0) && (s[r - i - 1] != kA0)) {
                    discr_r ^=
                        alpha_to[(index_of[lambda[i]] + s[r - i - 1]) % kNn];
                }
            }

            // Convert to index form
            discr_r = index_of[discr_r];

            if (discr_r == kA0) {
                // B(x) <-- x*B(x)
                memmove(&b[1], b, kNRoots * sizeof(b[0]));
                b[0] = kA0;
            } else {
                // T(x) <-- lambda(x) - discr_r*x*b(x)
                t[0] = lambda[0];
                for (i = 0; (uint32_t)i < kNRoots; i++) {
                    if (b[i] != kA0) {
                        t[i + 1] =
                            lambda[i + 1] ^ alpha_to[(discr_r + b[i]) % kNn];
                    } else {
                        t[i + 1] = lambda[i + 1];
                    }
                }

                if (2 * el <= r + no_eras - 1) {
                    el = r + no_eras - el;

                    // B(x) <-- inv(discr_r) * lambda(x)
                    for (i = 0; (uint32_t)i <= kNRoots; i++) {
                        b[i] =
                            (lambda[i] == 0)
                                ? kA0
                                : (index_of[lambda[i]] - discr_r + kNn) % kNn;
                    }
                } else {
                    // B(x) <-- x*B(x)
                    memmove(&b[1], b, kNRoots * sizeof(b[0]));
                    b[0] = kA0;
                }
                memcpy(lambda, t, (kNRoots + 1) * sizeof(t[0]));
            }
        }

        // Convert lambda to index form and compute deg(lambda(x))
        deg_lambda = 0;
        for (i = 0; i < kNRoots + 1; i++) {
            lambda[i] = index_of[lambda[i]];
            if (lambda[i] != kA0) {
                deg_lambda = i;
            }
        }

        // Find roots of the error+erasure locator polynomial by Chien search
        memcpy(&(reg[1]), &(lambda[1]), kNRoots * sizeof(reg[0]));
        count = 0;  // Number of roots of lambda(x)

        for (i = 1, k = kIPrim - 1; i <= kNn; i++, k = (k + kIPrim) % kNn) {
            q = 1;  // lambda[0] is always 0
            for (j = deg_lambda; j > 0; j--) {
                if (reg[j] != kA0) {
                    reg[j] = (reg[j] + j) % kNn;
                    q ^= alpha_to[reg[j]];
                }
            }

            if (q != 0) {
                continue;  // Not a root
            }

            // Store root (index-form) and error location number
            root[count] = i;
            loc[count] = k;

            // If we've already found max possible roots,
            // abort the search to save time
            //
            if (++count == deg_lambda) {
                break;
            }
        }

        if (deg_lambda != count) {
            //
            // deg(lambda) unequal to number of roots => uncorrectable
            // error detected
            //
            count = -1;
        } else {
            //
            // Compute err+eras evaluator poly omega(x) = s(x)*lambda(x) (modulo
            // x*kNRoots). in index form. Also find deg(omega).
            //
            deg_omega = 0;
            for (i = 0; i < kNRoots; i++) {
                tmp = 0;
                j = (deg_lambda < i) ? deg_lambda : i;
                for (; j >= 0; j--) {
                    if ((s[i - j] != kA0) && (lambda[j] != kA0)) {
                        tmp ^= alpha_to[(s[i - j] + lambda[j]) % kNn];
                    }
                }
                if (tmp != 0) {
                    deg_omega = i;
                }
                omega[i] = index_of[tmp];
            }
            omega[kNRoots] = kA0;

            //
            // Compute error values in poly-form. num1 = omega(inv(X(l))), num2
            // = inv(X(l))**(kFcr-1) and den = lambda_pr(inv(X(l))) all in
            // poly-form
            //
            for (j = count - 1; j >= 0; j--) {
                num1 = 0;
                for (i = deg_omega; i >= 0; i--) {
                    if (omega[i] != kA0) {
                        num1 ^= alpha_to[(omega[i] + i * root[j]) % kNn];
                    }
                }
                num2 = alpha_to[(root[j] * (kFcr - 1) + kNn) % kNn];
                den = 0;

                // lambda[i+1] for i even is the formal derivative lambda_pr of
                // lambda[i]
                for (i = static_cast<int16_t>(deg_lambda < kNRoots - 1
                                                  ? deg_lambda
                                                  : kNRoots - 1) &
                         ~static_cast<int16_t>(1);
                     i >= 0; i -= 2) {
                    if (lambda[i + 1] != kA0) {
                        den ^= alpha_to[(lambda[i + 1] + i * root[j]) % kNn];
                    }
                }
                if (den == 0) {
                    count = -1;
                } else {
                    // Apply error to data
                    if (num1 != 0) {
                        data[loc[j]] ^=
                            alpha_to[(index_of[num1] + index_of[num2] + kNn -
                                      index_of[den]) %
                                     kNn];
                    }
                }
            }
        }
    }

    if (eras_pos != NULL) {
        for (i = 0; i < count; i++) {
            eras_pos[i] = loc[i];
        }
    }

    return count;
}
