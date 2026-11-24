#include "app/demo_scenario.hpp"

#include <format>
#include <string>
#include <utility>

#include "application/ports/final_score_report.hpp"
#include "domain/errors.hpp"

namespace pos::app {

DemoScenario::DemoScenario(std::shared_ptr<application::PosFacade> facade,
                           std::ostream& out)
    : facade_(std::move(facade)), out_(out) {}

void DemoScenario::run(const std::filesystem::path& out_dir) const {
    out_ << "=== Демонстрационный сценарий модуля ПОС ===\n\n";

    out_ << "1. Студенты и рубрикатор\n";
    const auto ivanov = facade_->add_student("S-101", "Иванов Иван", "Б05-111");
    const auto petrova = facade_->add_student("S-102", "Петрова Мария", "Б05-112");
    const auto sidorov = facade_->add_student("S-103", "Сидоров Пётр", "Б05-111");
    out_ << std::format("   + {} ({}, {})\n", ivanov.full_name, ivanov.id, ivanov.group);
    out_ << std::format("   + {} ({}, {})\n", petrova.full_name, petrova.id, petrova.group);
    out_ << std::format("   + {} ({}, {})\n", sidorov.full_name, sidorov.id, sidorov.group);

    (void)facade_->add_criterion("attendance", "Посещаемость", 5, 20.0);
    (void)facade_->add_criterion("homework", "Домашние задания", 10, 50.0);
    (void)facade_->add_criterion("exam", "Экзамен", 10, 30.0);
    out_ << "   + критерии: attendance (max 5, вес 20), homework (max 10, вес 50), "
            "exam (max 10, вес 30)\n\n";

    out_ << "2. Выставление оценок\n";
    (void)facade_->record_evaluation("S-101", "attendance", 4, "Активная работа");
    (void)facade_->record_evaluation("S-101", "homework", 8);
    (void)facade_->record_evaluation("S-101", "exam", 9);
    (void)facade_->record_evaluation("S-102", "attendance", 5);
    (void)facade_->record_evaluation("S-102", "homework", 10);
    (void)facade_->record_evaluation("S-102", "exam", 7);
    (void)facade_->record_evaluation("S-103", "attendance", 3);
    (void)facade_->record_evaluation("S-103", "homework", 6);
    out_ << "   выставлено 8 оценок\n\n";

    out_ << "3. Проверка инвариантов\n";
    try {
        // У S-103 ещё нет оценки по экзамену, поэтому сработает именно
        // инвариант диапазона балла, а не проверка дубликата.
        (void)facade_->record_evaluation("S-103", "exam", 12);
        out_ << "   ОШИБКА: балл 12/10 был принят\n";
    } catch (const domain::DomainError& error) {
        out_ << "   ожидаемый отказ (балл 12/10): " << error.what() << '\n';
    }

    try {
        (void)facade_->record_evaluation("S-101", "attendance", 5);
        out_ << "   ОШИБКА: повторная оценка была принята\n";
    } catch (const domain::DomainError& error) {
        out_ << "   ожидаемый отказ (повторная оценка): " << error.what() << '\n';
    }
    out_ << '\n';

    out_ << "4. Исправление оценки S-101 по критерию homework: 8 -> 9\n";
    (void)facade_->correct_evaluation("S-101::homework", 9);
    out_ << "   исправлено\n\n";

    out_ << "5. Итог (взвешенное среднее, округление до 1 знака)\n";
    const auto finals = facade_->calculate_all_final_scores();
    out_ << std::format("   {:<8} {:<24} {:<10} {:>7} {:>10}\n",
                        "Зачётка", "Студент", "Группа", "Итог", "Оценено");
    for (const auto& row : finals) {
        out_ << std::format("   {:<8} {:<24} {:<10} {:>7.1f} {:>7}/{:<2}\n",
                            row.student_id,
                            row.student_name,
                            row.group,
                            row.percent,
                            row.assessed_criteria,
                            row.total_criteria);
    }
    out_ << '\n';

    const auto csv_path = out_dir / "final_scores.csv";
    const auto json_path = out_dir / "final_scores.json";
    facade_->export_report(application::ReportFormat::csv, csv_path);
    facade_->export_report(application::ReportFormat::json, json_path);

    out_ << "6. Экспорт итогов\n";
    out_ << "   CSV:  " << csv_path.string() << '\n';
    out_ << "   JSON: " << json_path.string() << '\n';
    out_ << "\n=== Демонстрация завершена ===\n";
}

}  // namespace pos::app
