#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "application/dto/dtos.hpp"
#include "domain/ports/evaluation_repository.hpp"
#include "domain/ports/rubricator_repository.hpp"
#include "domain/ports/student_repository.hpp"
#include "domain/services/final_score_calculator.hpp"

namespace pos::application {

class FinalScoreService {
public:
    FinalScoreService(std::shared_ptr<domain::StudentReader> students,
                      std::shared_ptr<domain::RubricatorReader> rubricators,
                      std::shared_ptr<domain::EvaluationReader> evaluations,
                      std::shared_ptr<domain::FinalScoreCalculator> calculator);

    [[nodiscard]] FinalScoreDto calculate_for_student(std::string_view student_id) const;
    [[nodiscard]] std::vector<FinalScoreDto> calculate_all() const;

private:
    std::shared_ptr<domain::StudentReader> students_;
    std::shared_ptr<domain::RubricatorReader> rubricators_;
    std::shared_ptr<domain::EvaluationReader> evaluations_;
    std::shared_ptr<domain::FinalScoreCalculator> calculator_;
};

}  // namespace pos::application
