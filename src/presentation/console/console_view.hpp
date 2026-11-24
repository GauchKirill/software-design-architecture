#pragma once

#include <ostream>
#include <span>

#include "application/dto/dtos.hpp"

namespace pos::presentation {

void print_students(std::ostream& out, std::span<const application::StudentDto> students);
void print_criteria(std::ostream& out, std::span<const application::CriterionDto> criteria);
void print_evaluations(std::ostream& out,
                       std::span<const application::EvaluationDto> evaluations);
void print_final_scores(std::ostream& out,
                        std::span<const application::FinalScoreDto> rows);

}  // namespace pos::presentation
