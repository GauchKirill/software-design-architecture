#pragma once

#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "application/dto/dtos.hpp"
#include "domain/ports/clock.hpp"
#include "domain/ports/evaluation_repository.hpp"
#include "domain/ports/rubricator_repository.hpp"
#include "domain/ports/student_repository.hpp"

namespace pos::application {

class EvaluationService {
public:
    EvaluationService(std::shared_ptr<domain::StudentReader> students,
                      std::shared_ptr<domain::RubricatorReader> rubricators,
                      std::shared_ptr<domain::EvaluationReader> evaluations,
                      std::shared_ptr<domain::EvaluationWriter> writer,
                      std::shared_ptr<domain::Clock> clock);

    [[nodiscard]] EvaluationDto record(std::string_view student_id,
                                       std::string_view criterion_id,
                                       int points,
                                       std::optional<std::string> comment = std::nullopt);
    [[nodiscard]] std::vector<EvaluationDto> list_for_student(
        std::string_view student_id) const;
    [[nodiscard]] EvaluationDto correct(std::string_view evaluation_id, int points);
    void cancel(std::string_view evaluation_id);

private:
    std::shared_ptr<domain::StudentReader> students_;
    std::shared_ptr<domain::RubricatorReader> rubricators_;
    std::shared_ptr<domain::EvaluationReader> evaluations_;
    std::shared_ptr<domain::EvaluationWriter> writer_;
    std::shared_ptr<domain::Clock> clock_;
};

}  // namespace pos::application
