#include <gtest/gtest.h>

#include <boost/json.hpp>
#include <filesystem>
#include <string>

#include "application/ports/final_score_report.hpp"
#include "support/service_fixture.hpp"

namespace {

using pos::application::ReportFormat;
using pos::test::ServiceFixture;

TEST(ReportServiceTest, RendersCsvWithHeaderAndTotals) {
    ServiceFixture fixture;
    fixture.seed();

    const auto csv = fixture.facade->render_report(ReportFormat::csv);
    EXPECT_NE(csv.find("student_id,full_name,group,percent,assessed,total"), std::string::npos);
    EXPECT_NE(csv.find("S-101"), std::string::npos);
    EXPECT_NE(csv.find("83.00"), std::string::npos);
    EXPECT_NE(csv.find("42.00"), std::string::npos);
}

TEST(ReportServiceTest, RendersValidJson) {
    ServiceFixture fixture;
    fixture.seed();

    const auto text = fixture.facade->render_report(ReportFormat::json);
    const auto parsed = boost::json::parse(text);
    const auto& rows = parsed.as_array();
    ASSERT_EQ(rows.size(), 3U);

    bool found = false;
    for (const auto& row : rows) {
        const auto& object = row.as_object();
        if (object.at("student_id").as_string() == "S-101") {
            found = true;
            EXPECT_DOUBLE_EQ(object.at("percent").as_double(), 83.0);
            EXPECT_EQ(object.at("assessed").as_int64(), 3);
            EXPECT_EQ(object.at("total").as_int64(), 3);
        }
    }
    EXPECT_TRUE(found);
}

TEST(ReportServiceTest, ExportsThroughReportWriter) {
    ServiceFixture fixture;
    fixture.seed();

    const std::filesystem::path target{"out/nested/final.csv"};
    fixture.facade->export_report(ReportFormat::csv, target);

    EXPECT_EQ(fixture.report_writer->path, target);
    EXPECT_NE(fixture.report_writer->content_copy.find("S-101"), std::string::npos);
}

}  // namespace
