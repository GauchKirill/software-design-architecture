#pragma once

#include <memory>

#include "domain/services/final_score_policy.hpp"

namespace pos::domain {

/// Структурный паттерн Decorator: добавляет округление поверх любой стратегии,
/// не меняя сами стратегии. Позволяет включать/выключать округление и задавать
/// число знаков при композиции приложения.
class RoundedFinalScorePolicy final : public FinalScorePolicy {
public:
    RoundedFinalScorePolicy(std::shared_ptr<const FinalScorePolicy> inner, int decimals);

    [[nodiscard]] FinalScore calculate(
        const Rubricator& rubricator,
        std::span<const Evaluation> evaluations) const override;

private:
    std::shared_ptr<const FinalScorePolicy> inner_;
    int decimals_;
};

}  // namespace pos::domain
