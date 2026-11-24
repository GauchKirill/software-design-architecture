#include "application/services/final_score_service.hpp"

#include <string>
#include <utility>

#include "application/mapping.hpp"
#include "domain/errors.hpp"
#include "domain/model/student_id.hpp"

namespace pos::application {

FinalScoreService::FinalScoreService(std::shared_ptr<domain::StudentReader> students,
                                     std::shared_ptr<domain::RubricatorReader> rubricators,
                                     std::shared_ptr<domain::EvaluationReader> evaluations,
                                     std::shared_ptr<domain::FinalScoreCalculator> calculator)
    : students_(std::move(students)),
      rubricators_(std::move(rubricators)),
      evaluations_(std::move(evaluations)),
      calculator_(std::move(calculator)) {}

FinalScoreDto FinalScoreService::calculate_for_student(std::string_view student_id) const {
    const domain::StudentId sid{std::string{student_id}};
    const auto student = students_->find_by_id(sid);
    if (!student.has_value()) {
        throw domain::NotFoundError("Студент '" + sid.value() + "' не найден");
    }

    const auto rubricator = rubricators_->load();
    const auto evaluations = evaluations_->find_by_student(sid);
    const auto result = calculator_->calculate(sid, rubricator, evaluations);
    return to_dto(*student, result);
}

std::vector<FinalScoreDto> FinalScoreService::calculate_all() const {
    const auto rubricator = rubricators_->load();
    std::vector<FinalScoreDto> result;
    for (const auto& student : students_->find_all()) {
        const auto evaluations = evaluations_->find_by_student(student.id());
        result.push_back(
            to_dto(student, calculator_->calculate(student.id(), rubricator, evaluations)));
    }
    return result;
}

}  // namespace pos::application
