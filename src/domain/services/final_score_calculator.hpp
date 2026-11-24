#pragma once

#include <cstddef>
#include <memory>
#include <span>

#include "domain/model/evaluation.hpp"
#include "domain/model/final_score.hpp"
#include "domain/model/rubricator.hpp"
#include "domain/model/student_id.hpp"
#include "domain/services/final_score_policy.hpp"

namespace pos::domain {

struct FinalScoreResult {
    FinalScore score;
    std::size_t assessed_criteria{0};
    std::size_t total_criteria{0};
};

/// Domain Service: правило «итог согласован с текущим набором оценок»
/// не принадлежит ни одному агрегату. Проверяет, что оценки относятся
/// к запрошенному студенту, существуют в рубрикаторе и не дублируются,
/// затем делегирует формулу выбранной Strategy.
class FinalScoreCalculator {
public:
    explicit FinalScoreCalculator(std::shared_ptr<FinalScorePolicy> policy);

    [[nodiscard]] FinalScoreResult calculate(
        const StudentId& student_id,
        const Rubricator& rubricator,
        std::span<const Evaluation> evaluations) const;

private:
    std::shared_ptr<FinalScorePolicy> policy_;
};

}  // namespace pos::domain
