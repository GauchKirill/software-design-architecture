#pragma once

#include "infrastructure/reports/final_score_report_template.hpp"

namespace pos::infrastructure {

class CsvFinalScoreReport final : public FinalScoreReportTemplate {
protected:
    void write_header(std::ostream& out) const override;
    void write_row(std::ostream& out,
                   const application::FinalScoreDto& row) const override;
    void write_footer(std::ostream& out) const override;
    [[nodiscard]] std::string_view row_separator() const override;
};

}  // namespace pos::infrastructure
