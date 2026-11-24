#include "domain/services/rounded_final_score_policy.hpp"

#include <cmath>
#include <utility>

#include "domain/errors.hpp"

namespace pos::domain {

RoundedFinalScorePolicy::RoundedFinalScorePolicy(
    std::shared_ptr<const FinalScorePolicy> inner,
    int decimals)
    : inner_(std::move(inner)), decimals_(decimals) {
    if (!inner_) {
        throw ValidationError("Rounding-декоратор требует базовую стратегию расчёта");
    }
    if (decimals_ < 0 || decimals_ > 4) {
        throw ValidationError("Число знаков округления должно быть в диапазоне 0..4");
    }
}

FinalScore RoundedFinalScorePolicy::calculate(
    const Rubricator& rubricator,
    std::span<const Evaluation> evaluations) const {
    const FinalScore base = inner_->calculate(rubricator, evaluations);
    const double factor = std::pow(10.0, static_cast<double>(decimals_));
    const double rounded = std::round(base.value() * factor) / factor;
    return FinalScore::percent(rounded);
}

}  // namespace pos::domain
