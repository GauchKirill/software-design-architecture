#pragma once

#include <chrono>
#include <format>
#include <string>

#include "application/dto/dtos.hpp"
#include "domain/model/criterion.hpp"
#include "domain/model/evaluation.hpp"
#include "domain/model/student.hpp"
#include "domain/services/final_score_calculator.hpp"

namespace pos::application {

[[nodiscard]] inline std::string format_date(std::chrono::year_month_day date) {
    return std::format("{:04d}-{:02d}-{:02d}",
                       static_cast<int>(date.year()),
                       static_cast<unsigned>(date.month()),
                       static_cast<unsigned>(date.day()));
}

[[nodiscard]] inline StudentDto to_dto(const domain::Student& student) {
    return StudentDto{
        .id = student.id().value(),
        .full_name = student.full_name().value(),
        .group = student.group().value(),
    };
}

[[nodiscard]] inline CriterionDto to_dto(const domain::Criterion& criterion) {
    return CriterionDto{
        .id = criterion.id().value(),
        .name = criterion.name(),
        .max_score = criterion.max_score().value(),
        .weight = criterion.weight().value(),
    };
}

[[nodiscard]] inline EvaluationDto to_dto(const domain::Evaluation& evaluation,
                                          const domain::Student& student,
                                          const domain::Criterion& criterion) {
    return EvaluationDto{
        .id = evaluation.id().value(),
        .student_id = student.id().value(),
        .student_name = student.full_name().value(),
        .criterion_id = criterion.id().value(),
        .criterion_name = criterion.name(),
        .points = evaluation.score().points(),
        .max_points = evaluation.score().max_points(),
        .date = format_date(evaluation.date()),
        .comment = evaluation.comment(),
    };
}

[[nodiscard]] inline FinalScoreDto to_dto(const domain::Student& student,
                                          const domain::FinalScoreResult& result) {
    return FinalScoreDto{
        .student_id = student.id().value(),
        .student_name = student.full_name().value(),
        .group = student.group().value(),
        .percent = result.score.value(),
        .assessed_criteria = result.assessed_criteria,
        .total_criteria = result.total_criteria,
    };
}

}  // namespace pos::application
