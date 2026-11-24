#include "presentation/console/console_view.hpp"

#include <format>
#include <ostream>
#include <span>

namespace pos::presentation {

void print_students(std::ostream& out,
                    std::span<const application::StudentDto> students) {
    if (students.empty()) {
        out << "  (пусто)\n";
        return;
    }
    out << std::format("  {:<10} {:<28} {:<12}\n", "Зачётка", "ФИО", "Группа");
    for (const auto& student : students) {
        out << std::format("  {:<10} {:<28} {:<12}\n",
                           student.id,
                           student.full_name,
                           student.group);
    }
}

void print_criteria(std::ostream& out,
                    std::span<const application::CriterionDto> criteria) {
    if (criteria.empty()) {
        out << "  (пусто)\n";
        return;
    }
    out << std::format("  {:<14} {:<28} {:>8} {:>8}\n",
                       "Код", "Название", "Максимум", "Вес");
    for (const auto& criterion : criteria) {
        out << std::format("  {:<14} {:<28} {:>8} {:>8.1f}\n",
                           criterion.id,
                           criterion.name,
                           criterion.max_score,
                           criterion.weight);
    }
}

void print_evaluations(std::ostream& out,
                       std::span<const application::EvaluationDto> evaluations) {
    if (evaluations.empty()) {
        out << "  (пусто)\n";
        return;
    }
    out << std::format("  {:<20} {:<12} {:<24} {:>7} {:<12} {}\n",
                       "ID оценки", "Зачётка", "Критерий", "Балл", "Дата", "Комментарий");
    for (const auto& evaluation : evaluations) {
        out << std::format("  {:<20} {:<12} {:<24} {:>3}/{:<3} {:<12} {}\n",
                           evaluation.id,
                           evaluation.student_id,
                           evaluation.criterion_name,
                           evaluation.points,
                           evaluation.max_points,
                           evaluation.date,
                           evaluation.comment.value_or(""));
    }
}

void print_final_scores(std::ostream& out,
                        std::span<const application::FinalScoreDto> rows) {
    if (rows.empty()) {
        out << "  (пусто)\n";
        return;
    }
    out << std::format("  {:<10} {:<28} {:<12} {:>7} {:>10}\n",
                       "Зачётка", "ФИО", "Группа", "Итог", "Оценено");
    for (const auto& row : rows) {
        out << std::format("  {:<10} {:<28} {:<12} {:>7.1f} {:>7}/{:<2}\n",
                           row.student_id,
                           row.student_name,
                           row.group,
                           row.percent,
                           row.assessed_criteria,
                           row.total_criteria);
    }
}

}  // namespace pos::presentation
