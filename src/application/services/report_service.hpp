#pragma once

#include "application/ports/final_score_report.hpp"
#include "application/services/final_score_service.hpp"

namespace pos::application {

class ReportService {
public:
    ReportService(std::shared_ptr<FinalScoreService> final_scores,
                  std::shared_ptr<FinalScoreReportFactory> reports,
                  std::shared_ptr<ReportWriter> writer);

    [[nodiscard]] std::string render(ReportFormat format) const;
    void export_report(ReportFormat format, const std::filesystem::path& path) const;

private:
    [[nodiscard]] std::vector<FinalScoreDto> collect_rows() const;

    std::shared_ptr<FinalScoreService> final_scores_;
    std::shared_ptr<FinalScoreReportFactory> reports_;
    std::shared_ptr<ReportWriter> writer_;
};

}  // namespace pos::application
