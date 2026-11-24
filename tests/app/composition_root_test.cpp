#include <gtest/gtest.h>

#include <string>

#include "app/app_config.hpp"
#include "app/composition_root.hpp"
#include "application/errors.hpp"

namespace {

using pos::app::AppConfig;
using pos::app::build_facade;
using pos::application::ApplicationError;

void seed(pos::application::PosFacade& facade) {
    (void)facade.add_student("S-101", "Иванов Иван", "Б05-111");
    (void)facade.add_criterion("attendance", "Посещаемость", 5, 20.0);
    (void)facade.add_criterion("homework", "Домашние задания", 10, 50.0);
    (void)facade.add_criterion("exam", "Экзамен", 10, 30.0);
    (void)facade.record_evaluation("S-101", "attendance", 4);
    (void)facade.record_evaluation("S-101", "homework", 8);
    (void)facade.record_evaluation("S-101", "exam", 9);
}

TEST(CompositionRootTest, BuildsWorkingGraphWithSharedRepositoryState) {
    AppConfig config;
    const auto facade = build_facade(config);

    seed(*facade);

    const auto students = facade->list_students();
    ASSERT_EQ(students.size(), 1U);
    EXPECT_EQ(students.front().id, "S-101");

    const auto score = facade->calculate_final_score("S-101");
    EXPECT_DOUBLE_EQ(score.percent, 83.0);
}

TEST(CompositionRootTest, StrategyIsSelectedByConfiguration) {
    AppConfig config;
    config.formula = "weighted-points";
    config.rounding = 1;
    const auto facade = build_facade(config);
    seed(*facade);

    EXPECT_DOUBLE_EQ(facade->calculate_final_score("S-101").percent, 83.3);
}

TEST(CompositionRootTest, DecoratorRoundingIsSelectedByConfiguration) {
    AppConfig config;
    config.formula = "weighted-points";
    config.rounding = 0;
    const auto facade = build_facade(config);
    seed(*facade);

    EXPECT_DOUBLE_EQ(facade->calculate_final_score("S-101").percent, 83.0);
}

TEST(CompositionRootTest, RejectsUnknownFormula) {
    AppConfig config;
    config.formula = "magic";
    EXPECT_THROW((void)build_facade(config), ApplicationError);
}

TEST(CompositionRootTest, RejectsInvalidRounding) {
    AppConfig config;
    config.rounding = 7;
    EXPECT_THROW((void)build_facade(config), ApplicationError);
}

}  // namespace
