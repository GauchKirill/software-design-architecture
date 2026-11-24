#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <utility>

#include "application/ports/final_score_report.hpp"

namespace pos::test {

/// Фейковый ReportWriter: запоминает содержимое вместо записи на диск.
class CapturingReportWriter final : public application::ReportWriter {
public:
    void write(const std::filesystem::path& target,
               std::string_view content) const override {
        path = target;
        content_copy = std::string{content};
    }

    mutable std::filesystem::path path;
    mutable std::string content_copy;
};

}  // namespace pos::test
