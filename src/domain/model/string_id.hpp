#pragma once

#include <string>

#include "domain/model/validation.hpp"

namespace pos::domain {

/// Общая реализация строкового идентификатора (Value Object).
/// Наследники задают тип-тег, чтобы StudentId и CriterionId нельзя было перепутать.
template <class Tag>
class StringId {
public:
    explicit StringId(std::string value) : value_(detail::trim_copy(value)) {
        detail::validate_identifier(value_);
    }

    [[nodiscard]] const std::string& value() const noexcept { return value_; }
    [[nodiscard]] std::string to_string() const { return value_; }

    auto operator<=>(const StringId&) const = default;
    bool operator==(const StringId&) const = default;

private:
    std::string value_;
};

}  // namespace pos::domain
