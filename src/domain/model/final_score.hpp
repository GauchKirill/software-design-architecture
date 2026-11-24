#pragma once

#include <cmath>

#include "domain/errors.hpp"

namespace pos::domain {

/// Итог по студенту в процентах (0..100). Считается Domain Service.
class FinalScore {
public:
    [[nodiscard]] static FinalScore percent(double value) {
        return FinalScore{value};
    }

    [[nodiscard]] double value() const noexcept { return value_; }

    auto operator<=>(const FinalScore&) const = default;
    bool operator==(const FinalScore&) const = default;

private:
    explicit FinalScore(double value) : value_(value) {
        if (!std::isfinite(value_) || value_ < 0.0 || value_ > 100.0) {
            throw ValidationError("Итог должен быть числом в диапазоне 0..100");
        }
    }

    double value_;
};

}  // namespace pos::domain
