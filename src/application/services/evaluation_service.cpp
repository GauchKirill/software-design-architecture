#include "application/services/evaluation_service.hpp"

#include <string>
#include <utility>

#include "application/mapping.hpp"
#include "domain/errors.hpp"
#include "domain/model/criterion_id.hpp"
#include "domain/model/evaluation.hpp"
#include "domain/model/student_id.hpp"
#include "domain/services/evaluation_factory.hpp"

namespace pos::application {

EvaluationService::EvaluationService(std::shared_ptr<domain::StudentReader> students,
                                     std::shared_ptr<domain::RubricatorReader> rubricators,
                                     std::shared_ptr<domain::EvaluationReader> evaluations,
                                     std::shared_ptr<domain::EvaluationWriter> writer,
                                     std::shared_ptr<domain::Clock> clock)
    : students_(std::move(students)),
      rubricators_(std::move(rubricators)),
      evaluations_(std::move(evaluations)),
      writer_(std::move(writer)),
      clock_(std::move(clock)) {}

EvaluationDto EvaluationService::record(std::string_view student_id,
                                        std::string_view criterion_id,
                                        int points,
                                        std::optional<std::string> comment) {
    const domain::StudentId sid{std::string{student_id}};
    const domain::CriterionId cid{std::string{criterion_id}};

    const auto student = students_->find_by_id(sid);
    if (!student.has_value()) {
        throw domain::NotFoundError("Студент '" + sid.value() + "' не найден");
    }

    const auto rubricator = rubricators_->load();
    const auto* criterion = rubricator.find(cid);
    if (criterion == nullptr) {
        throw domain::NotFoundError("Критерий '" + cid.value() +
                                    "' отсутствует в рубрикаторе");
    }

    if (evaluations_->find_by_student_and_criterion(sid, cid).has_value()) {
        throw domain::ConflictError(
            "По критерию '" + cid.value() + "' у студента '" + sid.value() +
            "' уже есть оценка. Используйте исправление оценки.");
    }

    const auto evaluation = domain::EvaluationFactory::create(
        *student, rubricator, cid, points, clock_->today(), std::move(comment));
    writer_->save(evaluation);
    return to_dto(evaluation, *student, *criterion);
}

std::vector<EvaluationDto> EvaluationService::list_for_student(
    std::string_view student_id) const {
    const domain::StudentId sid{std::string{student_id}};
    const auto student = students_->find_by_id(sid);
    if (!student.has_value()) {
        throw domain::NotFoundError("Студент '" + sid.value() + "' не найден");
    }

    const auto rubricator = rubricators_->load();
    std::vector<EvaluationDto> result;
    for (const auto& evaluation : evaluations_->find_by_student(sid)) {
        const auto* criterion = rubricator.find(evaluation.criterion_id());
        if (criterion == nullptr) {
            throw domain::ConflictError("Оценка '" + evaluation.id().value() +
                                        "' ссылается на отсутствующий критерий '" +
                                        evaluation.criterion_id().value() + "'");
        }
        result.push_back(to_dto(evaluation, *student, *criterion));
    }
    return result;
}

EvaluationDto EvaluationService::correct(std::string_view evaluation_id, int points) {
    const domain::EvaluationId eid{std::string{evaluation_id}};
    auto evaluation = evaluations_->find_by_id(eid);
    if (!evaluation.has_value()) {
        throw domain::NotFoundError("Оценка '" + eid.value() + "' не найдена");
    }

    const auto student = students_->find_by_id(evaluation->student_id());
    if (!student.has_value()) {
        throw domain::NotFoundError("Студент '" + evaluation->student_id().value() +
                                    "' не найден");
    }

    const auto rubricator = rubricators_->load();
    const auto* criterion = rubricator.find(evaluation->criterion_id());
    if (criterion == nullptr) {
        throw domain::NotFoundError("Критерий '" + evaluation->criterion_id().value() +
                                    "' отсутствует в рубрикаторе");
    }
    if (criterion->max_score().value() != evaluation->score().max_points()) {
        throw domain::ConflictError(
            "Максимум критерия изменился после выставления оценки: исправление невозможно "
            "без повторного согласования");
    }

    evaluation->rescore(points);
    writer_->save(*evaluation);
    return to_dto(*evaluation, *student, *criterion);
}

void EvaluationService::cancel(std::string_view evaluation_id) {
    const domain::EvaluationId eid{std::string{evaluation_id}};
    if (!evaluations_->find_by_id(eid).has_value()) {
        throw domain::NotFoundError("Оценка '" + eid.value() + "' не найдена");
    }
    writer_->remove(eid);
}

}  // namespace pos::application
