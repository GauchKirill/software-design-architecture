#pragma once

#include <chrono>
#include <memory>

#include "application/pos_facade.hpp"
#include "application/services/evaluation_service.hpp"
#include "application/services/final_score_service.hpp"
#include "application/services/report_service.hpp"
#include "application/services/rubricator_service.hpp"
#include "application/services/student_service.hpp"
#include "domain/services/final_score_calculator.hpp"
#include "domain/services/rounded_final_score_policy.hpp"
#include "domain/services/weighted_average_policy.hpp"
#include "infrastructure/persistence/in_memory_database.hpp"
#include "infrastructure/persistence/in_memory_evaluation_repository.hpp"
#include "infrastructure/persistence/in_memory_rubricator_repository.hpp"
#include "infrastructure/persistence/in_memory_student_repository.hpp"
#include "infrastructure/reports/report_factory_impl.hpp"
#include "support/capturing_report_writer.hpp"
#include "support/fixed_clock.hpp"

namespace pos::test {

/// Ручная сборка use-case сервисов для тестов application-слоя.
/// (В приложении этот граф собирает Boost.DI в composition root.)
struct ServiceFixture {
    std::shared_ptr<infrastructure::InMemoryDatabase> database =
        std::make_shared<infrastructure::InMemoryDatabase>();
    std::shared_ptr<FixedClock> clock = std::make_shared<FixedClock>(
        std::chrono::year_month_day{std::chrono::year{2026},
                                    std::chrono::month{2},
                                    std::chrono::day{1}});

    std::shared_ptr<infrastructure::InMemoryStudentRepository> student_repository =
        std::make_shared<infrastructure::InMemoryStudentRepository>(database);
    std::shared_ptr<infrastructure::InMemoryRubricatorRepository> rubricator_repository =
        std::make_shared<infrastructure::InMemoryRubricatorRepository>(database);
    std::shared_ptr<infrastructure::InMemoryEvaluationRepository> evaluation_repository =
        std::make_shared<infrastructure::InMemoryEvaluationRepository>(database);

    std::shared_ptr<domain::FinalScoreCalculator> calculator =
        std::make_shared<domain::FinalScoreCalculator>(
            std::make_shared<domain::RoundedFinalScorePolicy>(
                std::make_shared<domain::WeightedAveragePolicy>(), 1));

    std::shared_ptr<application::StudentService> students =
        std::make_shared<application::StudentService>(student_repository,
                                                      student_repository);
    std::shared_ptr<application::RubricatorService> rubricator =
        std::make_shared<application::RubricatorService>(rubricator_repository,
                                                         rubricator_repository);
    std::shared_ptr<application::EvaluationService> evaluations =
        std::make_shared<application::EvaluationService>(
            student_repository, rubricator_repository, evaluation_repository,
            evaluation_repository, clock);
    std::shared_ptr<application::FinalScoreService> final_scores =
        std::make_shared<application::FinalScoreService>(
            student_repository, rubricator_repository, evaluation_repository, calculator);

    std::shared_ptr<application::FinalScoreReportFactory> report_factory =
        std::make_shared<infrastructure::ReportFactoryImpl>();
    std::shared_ptr<CapturingReportWriter> report_writer =
        std::make_shared<CapturingReportWriter>();
    std::shared_ptr<application::ReportService> reports =
        std::make_shared<application::ReportService>(final_scores, report_factory,
                                                     report_writer);

    std::shared_ptr<application::PosFacade> facade =
        std::make_shared<application::PosFacade>(students, rubricator, evaluations,
                                                  final_scores, reports);

    void seed() {
        (void)facade->add_student("S-101", "Иванов Иван", "Б05-111");
        (void)facade->add_student("S-102", "Петрова Мария", "Б05-112");
        (void)facade->add_student("S-103", "Сидоров Пётр", "Б05-111");

        (void)facade->add_criterion("attendance", "Посещаемость", 5, 20.0);
        (void)facade->add_criterion("homework", "Домашние задания", 10, 50.0);
        (void)facade->add_criterion("exam", "Экзамен", 10, 30.0);

        (void)facade->record_evaluation("S-101", "attendance", 4);
        (void)facade->record_evaluation("S-101", "homework", 8);
        (void)facade->record_evaluation("S-101", "exam", 9);
        (void)facade->record_evaluation("S-102", "attendance", 5);
        (void)facade->record_evaluation("S-102", "homework", 10);
        (void)facade->record_evaluation("S-102", "exam", 7);
        (void)facade->record_evaluation("S-103", "attendance", 3);
        (void)facade->record_evaluation("S-103", "homework", 6);
    }
};

}  // namespace pos::test
