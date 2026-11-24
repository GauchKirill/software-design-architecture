#include "app/composition_root.hpp"

#include <memory>
#include <stdexcept>

#include <boost/di.hpp>

#include "app/app_config.hpp"
#include "application/pos_facade.hpp"
#include "application/services/evaluation_service.hpp"
#include "application/services/final_score_service.hpp"
#include "application/services/report_service.hpp"
#include "application/services/rubricator_service.hpp"
#include "application/services/student_service.hpp"
#include "domain/ports/clock.hpp"
#include "domain/ports/evaluation_repository.hpp"
#include "domain/ports/rubricator_repository.hpp"
#include "domain/ports/student_repository.hpp"
#include "domain/services/final_score_calculator.hpp"
#include "domain/services/final_score_policy.hpp"
#include "domain/services/rounded_final_score_policy.hpp"
#include "domain/services/weighted_average_policy.hpp"
#include "domain/services/weighted_points_policy.hpp"
#include "infrastructure/persistence/in_memory_database.hpp"
#include "infrastructure/persistence/in_memory_evaluation_repository.hpp"
#include "infrastructure/persistence/in_memory_rubricator_repository.hpp"
#include "infrastructure/persistence/in_memory_student_repository.hpp"
#include "infrastructure/reports/file_report_writer.hpp"
#include "infrastructure/reports/report_factory_impl.hpp"
#include "infrastructure/time/system_clock.hpp"

namespace pos::app {

std::shared_ptr<application::PosFacade> build_facade(const AppConfig& config) {
    validate_app_config(config);

    namespace di = boost::di;
    using namespace pos;

    const auto injector = di::make_injector(
        // Инфраструктура: один экземпляр БД на всё приложение.
        di::bind<infrastructure::InMemoryDatabase>.in(di::singleton),
        di::bind<domain::Clock>.to<infrastructure::SystemClock>().in(di::singleton),

        // Узкие порты → адаптеры. Reader и Writer — разные объекты,
        // но оба смотрят в один и тот же InMemoryDatabase.
        di::bind<domain::StudentReader>
            .to<infrastructure::InMemoryStudentRepository>()
            .in(di::singleton),
        di::bind<domain::StudentWriter>
            .to<infrastructure::InMemoryStudentRepository>()
            .in(di::singleton),
        di::bind<domain::RubricatorReader>
            .to<infrastructure::InMemoryRubricatorRepository>()
            .in(di::singleton),
        di::bind<domain::RubricatorWriter>
            .to<infrastructure::InMemoryRubricatorRepository>()
            .in(di::singleton),
        di::bind<domain::EvaluationReader>
            .to<infrastructure::InMemoryEvaluationRepository>()
            .in(di::singleton),
        di::bind<domain::EvaluationWriter>
            .to<infrastructure::InMemoryEvaluationRepository>()
            .in(di::singleton),

        // Strategy выбирается конфигурацией; Decorator добавляет округление.
        di::bind<domain::FinalScorePolicy>.to(
            [config](const auto&) -> std::shared_ptr<domain::FinalScorePolicy> {
                std::shared_ptr<domain::FinalScorePolicy> policy;
                if (config.formula == "weighted-average") {
                    policy = std::make_shared<domain::WeightedAveragePolicy>();
                } else {
                    policy = std::make_shared<domain::WeightedPointsPolicy>();
                }
                return std::make_shared<domain::RoundedFinalScorePolicy>(
                    policy, config.rounding);
            }),
        di::bind<domain::FinalScoreCalculator>.in(di::singleton),

        // Отчёты и файловый вывод.
        di::bind<application::FinalScoreReportFactory>
            .to<infrastructure::ReportFactoryImpl>()
            .in(di::singleton),
        di::bind<application::ReportWriter>
            .to<infrastructure::FileReportWriter>()
            .in(di::singleton),

        // Прикладные сценарии.
        di::bind<application::StudentService>.in(di::singleton),
        di::bind<application::RubricatorService>.in(di::singleton),
        di::bind<application::EvaluationService>.in(di::singleton),
        di::bind<application::FinalScoreService>.in(di::singleton),
        di::bind<application::ReportService>.in(di::singleton),
        di::bind<application::PosFacade>.in(di::singleton));

    return injector.create<std::shared_ptr<application::PosFacade>>();
}

}  // namespace pos::app
