#pragma once

#include <filesystem>
#include <memory>
#include <span>
#include <string>
#include <string_view>

#include "application/dto/dtos.hpp"
#include "application/errors.hpp"

namespace pos::application {

enum class ReportFormat { csv, json };

[[nodiscard]] inline std::string to_string(ReportFormat format) {
    switch (format) {
        case ReportFormat::csv:
            return "csv";
        case ReportFormat::json:
            return "json";
    }
    return "unknown";
}

[[nodiscard]] inline ReportFormat parse_report_format(std::string_view value) {
    if (value == "csv" || value == "CSV") {
        return ReportFormat::csv;
    }
    if (value == "json" || value == "JSON") {
        return ReportFormat::json;
    }
    throw ApplicationError("Неизвестный формат отчёта: '" + std::string{value} +
                           "' (ожидается csv или json)");
}

/// Порт отчёта: конкретный формат (CSV/JSON) реализуется в infrastructure.
class FinalScoreReport {
public:
    virtual ~FinalScoreReport() = default;
    [[nodiscard]] virtual std::string render(
        std::span<const FinalScoreDto> rows) const = 0;
};

/// Порождающая точка выбора реализации отчёта по формату.
class FinalScoreReportFactory {
public:
    virtual ~FinalScoreReportFactory() = default;
    [[nodiscard]] virtual std::unique_ptr<FinalScoreReport> create(
        ReportFormat format) const = 0;
};

/// Узкий порт записи готового отчёта (файл, память, облако — что угодно).
class ReportWriter {
public:
    virtual ~ReportWriter() = default;
    virtual void write(const std::filesystem::path& path,
                       std::string_view content) const = 0;
};

}  // namespace pos::application
