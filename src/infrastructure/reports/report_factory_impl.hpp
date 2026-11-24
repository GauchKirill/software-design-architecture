#pragma once

#include "application/ports/final_score_report.hpp"

namespace pos::infrastructure {

/// Concrete Factory: выбирает реализацию отчёта по формату.
/// Новый формат добавляется новым подклассом FinalScoreReportTemplate
/// и веткой здесь, не затрагивая Domain Service и Facade.
class ReportFactoryImpl final : public application::FinalScoreReportFactory {
public:
    [[nodiscard]] std::unique_ptr<application::FinalScoreReport> create(
        application::ReportFormat format) const override;
};

}  // namespace pos::infrastructure
