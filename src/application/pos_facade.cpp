#include "application/pos_facade.hpp"

#include <utility>

namespace pos::application {

PosFacade::PosFacade(std::shared_ptr<StudentService> students,
                     std::shared_ptr<RubricatorService> rubricator,
                     std::shared_ptr<EvaluationService> evaluations,
                     std::shared_ptr<FinalScoreService> final_scores,
                     std::shared_ptr<ReportService> reports)
    : students_(std::move(students)),
      rubricator_(std::move(rubricator)),
      evaluations_(std::move(evaluations)),
      final_scores_(std::move(final_scores)),
      reports_(std::move(reports)) {}

StudentDto PosFacade::add_student(std::string_view id,
                                  std::string_view full_name,
                                  std::string_view group) {
    return students_->add_student(id, full_name, group);
}

std::vector<StudentDto> PosFacade::list_students() const {
    return students_->list_students();
}

CriterionDto PosFacade::add_criterion(std::string_view id,
                                      std::string_view name,
                                      int max_score,
                                      double weight) {
    return rubricator_->add_criterion(id, name, max_score, weight);
}

std::vector<CriterionDto> PosFacade::list_criteria() const {
    return rubricator_->list_criteria();
}

EvaluationDto PosFacade::record_evaluation(std::string_view student_id,
                                           std::string_view criterion_id,
                                           int points,
                                           std::optional<std::string> comment) {
    return evaluations_->record(student_id, criterion_id, points, std::move(comment));
}

std::vector<EvaluationDto> PosFacade::list_evaluations(
    std::string_view student_id) const {
    return evaluations_->list_for_student(student_id);
}

EvaluationDto PosFacade::correct_evaluation(std::string_view evaluation_id, int points) {
    return evaluations_->correct(evaluation_id, points);
}

void PosFacade::cancel_evaluation(std::string_view evaluation_id) {
    evaluations_->cancel(evaluation_id);
}

FinalScoreDto PosFacade::calculate_final_score(std::string_view student_id) const {
    return final_scores_->calculate_for_student(student_id);
}

std::vector<FinalScoreDto> PosFacade::calculate_all_final_scores() const {
    return final_scores_->calculate_all();
}

std::string PosFacade::render_report(ReportFormat format) const {
    return reports_->render(format);
}

void PosFacade::export_report(ReportFormat format,
                              const std::filesystem::path& path) const {
    reports_->export_report(format, path);
}

}  // namespace pos::application
