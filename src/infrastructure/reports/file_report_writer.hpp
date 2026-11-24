#pragma once

#include "application/ports/final_score_report.hpp"

namespace pos::infrastructure {

/// Адаптер порта ReportWriter: пишет отчёт в файл, создавая каталоги.
/// File I/O и Boost.Filesystem живут только здесь, домен о них не знает.
class FileReportWriter final : public application::ReportWriter {
public:
    void write(const std::filesystem::path& path,
               std::string_view content) const override;
};

}  // namespace pos::infrastructure
