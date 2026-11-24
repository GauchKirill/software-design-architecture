#pragma once

#include "domain/services/final_score_policy.hpp"

namespace pos::domain {

/// Итог = сумма(вес_i * балл_i/максимум_i) / сумма(вес_i) * 100.
/// Каждый критерий сначала нормируется на свой максимум, затем взвешивается.
class WeightedAveragePolicy final : public FinalScorePolicy {
public:
    [[nodiscard]] FinalScore calculate(
        const Rubricator& rubricator,
        std::span<const Evaluation> evaluations) const override;
};

}  // namespace pos::domain
