#pragma once

#include <string>
#include <string_view>

#include "domain/model/validation.hpp"

namespace pos::domain {

class GroupName {
public:
    explicit GroupName(std::string value) : value_(detail::trim_copy(value)) {
        detail::require_text(value_, "Группа", 40);
    }

    [[nodiscard]] const std::string& value() const noexcept { return value_; }

    auto operator<=>(const GroupName&) const = default;
    bool operator==(const GroupName&) const = default;

private:
    std::string value_;
};

}  // namespace pos::domain
