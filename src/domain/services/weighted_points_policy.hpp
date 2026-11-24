#pragma once

#include "domain/services/final_score_policy.hpp"

namespace pos::domain {

/// Альтернативное правило: итог = сумма(балл_i * вес_i) / сумма(максимум_i * вес_i) * 100.
/// Отличается от WeightedAveragePolicy, когда у критериев разные максимумы:
/// здесь баллы складываются в «абсолютных» взвешенных единицах.
class WeightedPointsPolicy final : public FinalScorePolicy {
public:
    [[nodiscard]] FinalScore calculate(
        const Rubricator& rubricator,
        std::span<const Evaluation> evaluations) const override;
};

}  // namespace pos::domain
