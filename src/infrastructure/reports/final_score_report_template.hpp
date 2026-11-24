#pragma once

#include <ostream>
#include <span>
#include <string>
#include <string_view>

#include "application/ports/final_score_report.hpp"

namespace pos::infrastructure {

/// Поведенческий паттерн Template Method: общий каркас выгрузки итогов
/// (заголовок → строки с разделителями → подвал), а конкретные шаги
/// сериализации переопределяют CSV- и JSON-подклассы.
class FinalScoreReportTemplate : public application::FinalScoreReport {
public:
    [[nodiscard]] std::string render(
        std::span<const application::FinalScoreDto> rows) const final {
        std::ostringstream out;
        write_header(out);

        bool first = true;
        for (const auto& row : rows) {
            if (!first) {
                out << row_separator();
            }
            first = false;
            write_row(out, row);
        }

        write_footer(out);
        return out.str();
    }

protected:
    virtual void write_header(std::ostream& out) const = 0;
    virtual void write_row(std::ostream& out,
                           const application::FinalScoreDto& row) const = 0;
    virtual void write_footer(std::ostream& out) const = 0;
    [[nodiscard]] virtual std::string_view row_separator() const = 0;
};

}  // namespace pos::infrastructure
