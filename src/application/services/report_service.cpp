#include "application/services/report_service.hpp"

#include <utility>

namespace pos::application {

ReportService::ReportService(std::shared_ptr<FinalScoreService> final_scores,
                             std::shared_ptr<FinalScoreReportFactory> reports,
                             std::shared_ptr<ReportWriter> writer)
    : final_scores_(std::move(final_scores)),
      reports_(std::move(reports)),
      writer_(std::move(writer)) {}

std::vector<FinalScoreDto> ReportService::collect_rows() const {
    return final_scores_->calculate_all();
}

std::string ReportService::render(ReportFormat format) const {
    const auto report = reports_->create(format);
    const auto rows = collect_rows();
    return report->render(rows);
}

void ReportService::export_report(ReportFormat format,
                                  const std::filesystem::path& path) const {
    writer_->write(path, render(format));
}

}  // namespace pos::application
