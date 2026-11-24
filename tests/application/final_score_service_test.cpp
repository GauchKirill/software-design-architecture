#include <gtest/gtest.h>

#include <string>

#include "domain/errors.hpp"
#include "support/service_fixture.hpp"

namespace {

using pos::domain::NotFoundError;
using pos::test::ServiceFixture;

TEST(FinalScoreServiceTest, CalculatesWeightedAverageForStudent) {
    ServiceFixture fixture;
    fixture.seed();

    const auto result = fixture.facade->calculate_final_score("S-101");
    EXPECT_DOUBLE_EQ(result.percent, 83.0);
    EXPECT_EQ(result.assessed_criteria, 3U);
    EXPECT_EQ(result.total_criteria, 3U);
}

TEST(FinalScoreServiceTest, MissingCriteriaReduceScoreAndAssessedCount) {
    ServiceFixture fixture;
    fixture.seed();

    const auto result = fixture.facade->calculate_final_score("S-103");
    EXPECT_DOUBLE_EQ(result.percent, 42.0);
    EXPECT_EQ(result.assessed_criteria, 2U);
    EXPECT_EQ(result.total_criteria, 3U);
}

TEST(FinalScoreServiceTest, CalculatesAllStudents) {
    ServiceFixture fixture;
    fixture.seed();

    const auto rows = fixture.facade->calculate_all_final_scores();
    ASSERT_EQ(rows.size(), 3U);
    EXPECT_EQ(rows[0].student_id, "S-101");
    EXPECT_EQ(rows[1].student_id, "S-102");
    EXPECT_EQ(rows[2].student_id, "S-103");
}

TEST(FinalScoreServiceTest, RejectsUnknownStudent) {
    ServiceFixture fixture;
    fixture.seed();

    EXPECT_THROW((void)fixture.facade->calculate_final_score("S-999"), NotFoundError);
}

TEST(FinalScoreServiceTest, EmptyRubricatorIsReported) {
    ServiceFixture fixture;
    (void)fixture.facade->add_student("S-101", "Иванов Иван", "Б05-111");

    EXPECT_THROW((void)fixture.facade->calculate_final_score("S-101"),
                 pos::domain::ValidationError);
}

}  // namespace
