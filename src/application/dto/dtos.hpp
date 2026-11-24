#pragma once

#include <cstddef>
#include <optional>
#include <string>

namespace pos::application {

/// DTO для границы приложения.
/// Они нужны только для передачи данных в консоль/отчёты.
struct StudentDto {
    std::string id;
    std::string full_name;
    std::string group;
};

struct CriterionDto {
    std::string id;
    std::string name;
    int max_score{0};
    double weight{0.0};
};

struct EvaluationDto {
    std::string id;
    std::string student_id;
    std::string student_name;
    std::string criterion_id;
    std::string criterion_name;
    int points{0};
    int max_points{0};
    std::string date;
    std::optional<std::string> comment;
};

struct FinalScoreDto {
    std::string student_id;
    std::string student_name;
    std::string group;
    double percent{0.0};
    std::size_t assessed_criteria{0};
    std::size_t total_criteria{0};
};

}  // namespace pos::application
