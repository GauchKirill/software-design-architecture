#include "infrastructure/reports/json_final_score_report.hpp"

#include <cstdint>
#include <format>
#include <string>
#include <string_view>

#include <boost/json.hpp>

namespace pos::infrastructure {
namespace {

/// Экранирование строки средствами Boost.JSON (кавычки, переводы строк,
/// управляющие символы). Числа форматируем отдельно, чтобы в отчёте были
/// обычные десятичные значения, а не экспоненциальная запись.
std::string json_quote(std::string_view value) {
    return boost::json::serialize(boost::json::value(std::string{value}));
}

}  // namespace

void JsonFinalScoreReport::write_header(std::ostream& out) const {
    out << "[\n";
}

void JsonFinalScoreReport::write_row(
    std::ostream& out,
    const application::FinalScoreDto& row) const {
    out << "  {"
        << "\"student_id\":" << json_quote(row.student_id)
        << ",\"full_name\":" << json_quote(row.student_name)
        << ",\"group\":" << json_quote(row.group)
        << ",\"percent\":" << std::format("{:.2f}", row.percent)
        << ",\"assessed\":" << row.assessed_criteria
        << ",\"total\":" << row.total_criteria
        << '}';
}

void JsonFinalScoreReport::write_footer(std::ostream& out) const {
    out << "\n]\n";
}

std::string_view JsonFinalScoreReport::row_separator() const {
    return ",\n";
}

}  // namespace pos::infrastructure

