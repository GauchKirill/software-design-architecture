#include "infrastructure/reports/report_factory_impl.hpp"

#include <memory>

#include "infrastructure/reports/csv_final_score_report.hpp"
#include "infrastructure/reports/json_final_score_report.hpp"

namespace pos::infrastructure {

std::unique_ptr<application::FinalScoreReport> ReportFactoryImpl::create(
    application::ReportFormat format) const {
    switch (format) {
        case application::ReportFormat::csv:
            return std::make_unique<CsvFinalScoreReport>();
        case application::ReportFormat::json:
            return std::make_unique<JsonFinalScoreReport>();
    }
    throw application::ApplicationError("Неизвестный формат отчёта");
}

}  // namespace pos::infrastructure
