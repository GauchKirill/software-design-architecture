#pragma once

#include "domain/errors.hpp"

namespace pos::domain {

/// Максимальный балл критерия: целое число от 1 до 1000.
class MaxScore {
public:
    explicit MaxScore(int value) : value_(value) {
        if (value_ <= 0) {
            throw ValidationError("Максимальный балл критерия должен быть больше нуля");
        }
        if (value_ > 1000) {
            throw ValidationError("Максимальный балл критерия не должен превышать 1000");
        }
    }

    [[nodiscard]] int value() const noexcept { return value_; }

    auto operator<=>(const MaxScore&) const = default;
    bool operator==(const MaxScore&) const = default;

private:
    int value_;
};

}  // namespace pos::domain
