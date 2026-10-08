#pragma once

#include <charconv>
#include <string>
#include <type_traits>

// How a primitive becomes text: its fields in order, in parentheses, separated by ", ", as in
// "(1.5, -2)" (docs/layout.md, 3). Numbers go through std::to_chars, which ignores the locale, so the
// decimal point is always a point. A floating point number is written in the shortest form that reads
// back as the same value, never in scientific notation: 100000 stays 100000, not 1e+05.
template<typename T>
void pxAppendNumber(std::string& out, T value) {
    // The longest double in fixed notation (the smallest denormal, 0.000...5) has 326 characters.
    char buffer[512];
    std::to_chars_result result;
    if constexpr (std::is_floating_point_v<T>)
        result = std::to_chars(buffer, buffer + sizeof buffer, value, std::chars_format::fixed);
    else
        result = std::to_chars(buffer, buffer + sizeof buffer, value);
    out.append(buffer, result.ptr);
}

template<typename... Ts>
std::string pxTupleString(Ts... values) {
    std::string out = "(";
    bool first = true;
    ((out += first ? "" : ", ", first = false, pxAppendNumber(out, values)), ...);
    out += ')';
    return out;
}
