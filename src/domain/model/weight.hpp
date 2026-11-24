#pragma once

#include <cmath>

#include "domain/errors.hpp"

namespace pos::domain {

/// Вклад критерия в итог. Положительное конечное число.
class Weight {
public:
    explicit Weight(double value) : value_(value) {
        if (!std::isfinite(value_) || value_ <= 0.0) {
            throw ValidationError("Вес критерия должен быть положительным числом");
        }
        if (value_ > 1000.0) {
            throw ValidationError("Вес критерия не должен превышать 1000");
        }
    }

    [[nodiscard]] double value() const noexcept { return value_; }

    auto operator<=>(const Weight&) const = default;
    bool operator==(const Weight&) const = default;

private:
    double value_;
};

}  // namespace pos::domain
