#include <gtest/gtest.h>

#include <boost/json.hpp>
#include <string>
#include <vector>

#include "infrastructure/reports/csv_final_score_report.hpp"
#include "infrastructure/reports/json_final_score_report.hpp"

namespace {

using pos::application::FinalScoreDto;
using pos::infrastructure::CsvFinalScoreReport;
using pos::infrastructure::JsonFinalScoreReport;

FinalScoreDto make_row() {
    return FinalScoreDto{
        .student_id = "S-101",
        .student_name = "Иванов, \"Иван\"",
        .group = "Б05-111",
        .percent = 83.0,
        .assessed_criteria = 3,
        .total_criteria = 3,
    };
}

TEST(CsvFinalScoreReportTest, EscapesCommasAndQuotes) {
    const std::vector<FinalScoreDto> rows{make_row()};
    const CsvFinalScoreReport report;

    const auto csv = report.render(rows);
    EXPECT_NE(csv.find("student_id,full_name,group,percent,assessed,total"), std::string::npos);
    EXPECT_NE(csv.find("\"Иванов, \"\"Иван\"\"\""), std::string::npos);
    EXPECT_NE(csv.find("83.00,3,3"), std::string::npos);
}

TEST(JsonFinalScoreReportTest, ProducesValidJsonArray) {
    const std::vector<FinalScoreDto> rows{make_row()};
    const JsonFinalScoreReport report;

    const auto parsed = boost::json::parse(report.render(rows));
    const auto& array = parsed.as_array();
    ASSERT_EQ(array.size(), 1U);

    const auto& object = array.front().as_object();
    EXPECT_EQ(object.at("student_id").as_string(), "S-101");
    EXPECT_EQ(object.at("full_name").as_string(), "Иванов, \"Иван\"");
    EXPECT_DOUBLE_EQ(object.at("percent").as_double(), 83.0);
}

TEST(JsonFinalScoreReportTest, EmptyInputIsStillValidJson) {
    const JsonFinalScoreReport report;
    const auto parsed = boost::json::parse(report.render({}));
    EXPECT_TRUE(parsed.as_array().empty());
}

}  // namespace
