#pragma once

#include <string>

#include "domain/errors.hpp"

namespace pos::domain {

/// Балл вместе с границей критерия. Инвариант 0 <= points <= max_points
/// охраняется внутри самого Value Object, поэтому невалидный Score
/// невозможно создать ни из фабрики, ни при исправлении оценки.
class Score {
public:
    [[nodiscard]] static Score of(int points, int max_points) {
        return Score{points, max_points};
    }

    [[nodiscard]] int points() const noexcept { return points_; }
    [[nodiscard]] int max_points() const noexcept { return max_points_; }

    [[nodiscard]] double ratio() const noexcept {
        return static_cast<double>(points_) / static_cast<double>(max_points_);
    }

    auto operator<=>(const Score&) const = default;
    bool operator==(const Score&) const = default;

private:
    Score(int points, int max_points) : points_(points), max_points_(max_points) {
        if (max_points_ <= 0) {
            throw ValidationError("Максимальный балл должен быть больше нуля");
        }
        if (points_ < 0 || points_ > max_points_) {
            throw ValidationError("Балл " + std::to_string(points_) +
                                  " выходит за пределы 0.." + std::to_string(max_points_));
        }
    }

    int points_;
    int max_points_;
};

}  // namespace pos::domain
