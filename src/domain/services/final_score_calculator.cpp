#include "domain/services/final_score_calculator.hpp"

#include <string>
#include <unordered_set>
#include <utility>

#include "domain/errors.hpp"

namespace pos::domain {

FinalScoreCalculator::FinalScoreCalculator(std::shared_ptr<FinalScorePolicy> policy)
    : policy_(std::move(policy)) {
    if (!policy_) {
        throw ValidationError("FinalScoreCalculator требует стратегию расчёта итога");
    }
}

FinalScoreResult FinalScoreCalculator::calculate(
    const StudentId& student_id,
    const Rubricator& rubricator,
    std::span<const Evaluation> evaluations) const {
    if (rubricator.empty()) {
        throw ValidationError("Рубрикатор пуст: итог посчитать невозможно");
    }

    std::unordered_set<std::string> seen_criteria;
    for (const auto& evaluation : evaluations) {
        if (!(evaluation.student_id() == student_id)) {
            throw ValidationError("Оценка " + evaluation.id().value() +
                                  " относится к другому студенту");
        }
        if (!rubricator.contains(evaluation.criterion_id())) {
            throw ValidationError("Оценка ссылается на критерий '" +
                                  evaluation.criterion_id().value() +
                                  "', которого нет в рубрикаторе");
        }
        const auto [it, inserted] = seen_criteria.insert(evaluation.criterion_id().value());
        (void)it;
        if (!inserted) {
            throw ConflictError("По критерию '" + evaluation.criterion_id().value() +
                                "' найдено несколько актуальных оценок");
        }
    }

    return FinalScoreResult{
        policy_->calculate(rubricator, evaluations),
        evaluations.size(),
        rubricator.size(),
    };
}

}  // namespace pos::domain
