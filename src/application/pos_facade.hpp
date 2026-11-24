#pragma once

#include <optional>
#include <string_view>

#include "application/dto/dtos.hpp"
#include "application/ports/final_score_report.hpp"
#include "application/services/evaluation_service.hpp"
#include "application/services/final_score_service.hpp"
#include "application/services/report_service.hpp"
#include "application/services/rubricator_service.hpp"
#include "application/services/student_service.hpp"

namespace pos::application {

/// Структурный паттерн Facade: консоль работает с одним узким API и не знает
/// про пять use-case сервисов, репозитории и Domain Service.
class PosFacade {
public:
    PosFacade(std::shared_ptr<StudentService> students,
              std::shared_ptr<RubricatorService> rubricator,
              std::shared_ptr<EvaluationService> evaluations,
              std::shared_ptr<FinalScoreService> final_scores,
              std::shared_ptr<ReportService> reports);

    [[nodiscard]] StudentDto add_student(std::string_view id,
                                         std::string_view full_name,
                                         std::string_view group);
    [[nodiscard]] std::vector<StudentDto> list_students() const;

    [[nodiscard]] CriterionDto add_criterion(std::string_view id,
                                              std::string_view name,
                                              int max_score,
                                              double weight);
    [[nodiscard]] std::vector<CriterionDto> list_criteria() const;

    [[nodiscard]] EvaluationDto record_evaluation(
        std::string_view student_id,
        std::string_view criterion_id,
        int points,
        std::optional<std::string> comment = std::nullopt);
    [[nodiscard]] std::vector<EvaluationDto> list_evaluations(
        std::string_view student_id) const;
    [[nodiscard]] EvaluationDto correct_evaluation(std::string_view evaluation_id,
                                                   int points);
    void cancel_evaluation(std::string_view evaluation_id);

    [[nodiscard]] FinalScoreDto calculate_final_score(std::string_view student_id) const;
    [[nodiscard]] std::vector<FinalScoreDto> calculate_all_final_scores() const;

    [[nodiscard]] std::string render_report(ReportFormat format) const;
    void export_report(ReportFormat format, const std::filesystem::path& path) const;

private:
    std::shared_ptr<StudentService> students_;
    std::shared_ptr<RubricatorService> rubricator_;
    std::shared_ptr<EvaluationService> evaluations_;
    std::shared_ptr<FinalScoreService> final_scores_;
    std::shared_ptr<ReportService> reports_;
};

}  // namespace pos::application
