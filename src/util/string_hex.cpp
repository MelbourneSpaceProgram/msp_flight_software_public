#include <external/etl/exception.h>
#include <src/util/string_hex.h>

// Prepend 0x to each contiguous pair of chars (representing a single hex
// value), to return a hex formatted string.
std::string StringHex::StringToHex(const std::string& input) {
    static constexpr char* const lut = "0123456789abcdef";
    size_t len = input.length();

    if (len & 1) {
        throw etl::exception("Odd length", __FILE__, __LINE__);
    }

    std::string output;
    output.reserve(len / 2);

    for (size_t i = 0; i < len; i += 2) {
        char a = input[i];
        const char* p = std::lower_bound(lut, lut + 16, a);

        if (*p != a) {
            throw etl::exception("Not a hex digit", __FILE__, __LINE__);
        }
        char b = input[i + 1];
        const char* q = std::lower_bound(lut, lut + 16, b);

        if (*q != b) {
            throw etl::exception("Not a hex digit", __FILE__, __LINE__);
        }
        output.push_back(((p - lut) << 4) | (q - lut));
    }

    return output;
}
