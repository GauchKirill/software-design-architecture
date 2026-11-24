#pragma once

#include <string>
#include <string_view>

#include "domain/model/validation.hpp"

namespace pos::domain {

/// ФИО студента. Value Object: нормализуется (trim) и проверяется при создании.
class FullName {
public:
    explicit FullName(std::string value) : value_(detail::trim_copy(value)) {
        detail::require_text(value_, "ФИО", 120);
    }

    [[nodiscard]] const std::string& value() const noexcept { return value_; }

    auto operator<=>(const FullName&) const = default;
    bool operator==(const FullName&) const = default;

private:
    std::string value_;
};

}  // namespace pos::domain
