#pragma once

#include <cctype>
#include <optional>
#include <string>
#include <string_view>

#include "domain/errors.hpp"

namespace pos::domain::detail {

inline std::string trim_copy(std::string_view text) {
    std::size_t begin = 0;
    while (begin < text.size() && std::isspace(static_cast<unsigned char>(text[begin])) != 0) {
        ++begin;
    }
    std::size_t end = text.size();
    while (end > begin && std::isspace(static_cast<unsigned char>(text[end - 1])) != 0) {
        --end;
    }
    return std::string{text.substr(begin, end - begin)};
}

inline void require_text(std::string_view value,
                         std::string_view field,
                         std::size_t max_length,
                         bool allow_newlines = false) {
    if (value.empty()) {
        throw ValidationError(std::string{field} + " не может быть пустым");
    }
    if (value.size() > max_length) {
        throw ValidationError(std::string{field} + " длиннее " + std::to_string(max_length) +
                              " символов");
    }
    for (char ch : value) {
        const auto byte = static_cast<unsigned char>(ch);
        if (byte < 0x20U && !(allow_newlines && (ch == '\n' || ch == '\t'))) {
            throw ValidationError(std::string{field} + " содержит управляющий символ");
        }
    }
}

inline void validate_identifier(std::string_view value) {
    require_text(value, "Идентификатор", 40);
    for (char ch : value) {
        if (std::isspace(static_cast<unsigned char>(ch)) != 0 || ch == ':') {
            throw ValidationError(
                "Идентификатор не должен содержать пробелы и символ ':' (получено: " +
                std::string{value} + ")");
        }
    }
}

inline std::optional<std::string> normalize_comment(std::optional<std::string> comment) {
    if (!comment.has_value()) {
        return std::nullopt;
    }
    auto trimmed = trim_copy(*comment);
    if (trimmed.empty()) {
        return std::nullopt;
    }
    require_text(trimmed, "Комментарий", 500, true);
    return trimmed;
}

}  // namespace pos::domain::detail
