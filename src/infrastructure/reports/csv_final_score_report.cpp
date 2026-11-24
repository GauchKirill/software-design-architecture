#include "infrastructure/reports/csv_final_score_report.hpp"

#include <format>
#include <string>
#include <string_view>

namespace pos::infrastructure {
namespace {

std::string csv_escape(std::string_view value) {
    const bool needs_quotes = value.find_first_of(",\"\n\r") != std::string_view::npos;
    if (!needs_quotes) {
        return std::string{value};
    }

    std::string escaped;
    escaped.reserve(value.size() + 2);
    escaped.push_back('"');
    for (char ch : value) {
        if (ch == '"') {
            escaped.push_back('"');
        }
        escaped.push_back(ch);
    }
    escaped.push_back('"');
    return escaped;
}

}  // namespace

void CsvFinalScoreReport::write_header(std::ostream& out) const {
    out << "student_id,full_name,group,percent,assessed,total\n";
}

void CsvFinalScoreReport::write_row(
    std::ostream& out,
    const application::FinalScoreDto& row) const {
    out << csv_escape(row.student_id) << ','
        << csv_escape(row.student_name) << ','
        << csv_escape(row.group) << ','
        << std::format("{:.2f}", row.percent) << ','
        << row.assessed_criteria << ','
        << row.total_criteria;
}

void CsvFinalScoreReport::write_footer(std::ostream& out) const {
    out << '\n';
}

std::string_view CsvFinalScoreReport::row_separator() const {
    return "\n";
}

}  // namespace pos::infrastructure
