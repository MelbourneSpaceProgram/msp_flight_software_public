#include <src/util/string_hex.h>

// Prepend 0x to each contiguous pair of chars (representing a single hex
// value), to return a hex formatted string.
std::string StringHex::StringToHex(const std::string& input) {
    static const char* const lut = "0123456789abcdef";
    size_t len = input.length();

    if (len & 1) {
        throw std::invalid_argument("odd length");
    }

    std::string output;
    output.reserve(len / 2);

    for (size_t i = 0; i < len; i += 2) {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);

        if (*p != a) {
            throw std::invalid_argument("not a hex digit");
        }
        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);

        if (*q != b) {
            throw std::invalid_argument("not a hex digit");
        }
        output.push_back(((p - lut) << 4) | (q - lut));
    }

    return output;
}
