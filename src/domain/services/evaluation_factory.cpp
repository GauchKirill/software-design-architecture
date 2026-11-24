#include "domain/services/evaluation_factory.hpp"

#include <utility>

#include "domain/errors.hpp"

namespace pos::domain {

Evaluation EvaluationFactory::create(const Student& student,
                                     const Rubricator& rubricator,
                                     const CriterionId& criterion_id,
                                     int points,
                                     std::chrono::year_month_day date,
                                     std::optional<std::string> comment) {
    if (!date.ok()) {
        throw ValidationError("Некорректная дата оценки");
    }

    const Criterion* criterion = rubricator.find(criterion_id);
    if (criterion == nullptr) {
        throw NotFoundError("Критерий '" + criterion_id.value() +
                            "' отсутствует в рубрикаторе");
    }

    // Score::of охраняет инвариант 0 <= points <= max_score критерия.
    const auto score = Score::of(points, criterion->max_score().value());
    auto normalized_comment = detail::normalize_comment(std::move(comment));

    return Evaluation{EvaluationId::for_pair(student.id(), criterion->id()),
                      student.id(),
                      criterion->id(),
                      score,
                      date,
                      std::move(normalized_comment)};
}

}  // namespace pos::domain

