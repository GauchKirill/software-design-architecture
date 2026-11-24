#pragma once

#include <span>

#include "domain/model/evaluation.hpp"
#include "domain/model/final_score.hpp"
#include "domain/model/rubricator.hpp"

namespace pos::domain {

/// Поведенческий паттерн Strategy: разные правила расчёта итога.
/// Реализации обязаны вернуть FinalScore в диапазоне 0..100 и учитывать
/// все критерии рубрикатора (неоценённые — как 0 баллов).
class FinalScorePolicy {
public:
    virtual ~FinalScorePolicy() = default;

    [[nodiscard]] virtual FinalScore calculate(
        const Rubricator& rubricator,
        std::span<const Evaluation> evaluations) const = 0;
};

}  // namespace pos::domain
