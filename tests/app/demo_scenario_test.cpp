#include <gtest/gtest.h>

#include <boost/json.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "app/app_config.hpp"
#include "app/composition_root.hpp"
#include "app/demo_scenario.hpp"
#include "support/temp_dir.hpp"

namespace {

using pos::app::AppConfig;
using pos::app::build_facade;
using pos::app::DemoScenario;
using pos::test::TempDir;

TEST(DemoScenarioTest, RunsEndToEndAndExportsTotals) {
    AppConfig config;
    config.formula = "weighted-average";
    config.rounding = 1;

    const auto facade = build_facade(config);
    const TempDir directory;

    std::ostringstream output;
    const DemoScenario demo{facade, output};
    demo.run(directory.path());

    const auto text = output.str();
    EXPECT_NE(text.find("Иванов"), std::string::npos);
    EXPECT_NE(text.find("88.0"), std::string::npos);
    EXPECT_NE(text.find("91.0"), std::string::npos);
    EXPECT_NE(text.find("42.0"), std::string::npos);
    EXPECT_NE(text.find("ожидаемый отказ"), std::string::npos);

    const auto csv_path = directory.path() / "final_scores.csv";
    const auto json_path = directory.path() / "final_scores.json";
    ASSERT_TRUE(std::filesystem::exists(csv_path));
    ASSERT_TRUE(std::filesystem::exists(json_path));

    std::ifstream input{json_path};
    std::stringstream buffer;
    buffer << input.rdbuf();
    const auto parsed = boost::json::parse(buffer.str());
    ASSERT_EQ(parsed.as_array().size(), 3U);
}

}  // namespace
