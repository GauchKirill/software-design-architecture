#include <gtest/gtest.h>

#include <algorithm>
#include <string>

#include "domain/errors.hpp"
#include "support/service_fixture.hpp"

namespace {

using namespace pos::domain;
using pos::test::ServiceFixture;

TEST(EvaluationServiceTest, RecordsAndListsEvaluationsWithCriterionNames) {
    ServiceFixture fixture;
    fixture.seed();

    const auto evaluations = fixture.facade->list_evaluations("S-101");
    ASSERT_EQ(evaluations.size(), 3U);

    const auto has_exam = std::any_of(
        evaluations.begin(), evaluations.end(), [](const auto& evaluation) {
            return evaluation.criterion_id == "exam" && evaluation.criterion_name == "Экзамен";
        });
    EXPECT_TRUE(has_exam);
}

TEST(EvaluationServiceTest, RejectsUnknownStudent) {
    ServiceFixture fixture;
    fixture.seed();

    EXPECT_THROW((void)fixture.facade->record_evaluation("S-999", "exam", 5), NotFoundError);
}

TEST(EvaluationServiceTest, RejectsUnknownCriterion) {
    ServiceFixture fixture;
    fixture.seed();

    EXPECT_THROW((void)fixture.facade->record_evaluation("S-101", "unknown", 5),
                 NotFoundError);
}

TEST(EvaluationServiceTest, RejectsDuplicateEvaluationForPair) {
    ServiceFixture fixture;
    fixture.seed();

    EXPECT_THROW((void)fixture.facade->record_evaluation("S-101", "exam", 10),
                 ConflictError);
}

TEST(EvaluationServiceTest, CorrectsEvaluationWithinCriterionRange) {
    ServiceFixture fixture;
    fixture.seed();

    const auto corrected = fixture.facade->correct_evaluation("S-101::homework", 9);
    EXPECT_EQ(corrected.points, 9);
    EXPECT_EQ(corrected.max_points, 10);
    EXPECT_DOUBLE_EQ(fixture.facade->calculate_final_score("S-101").percent, 88.0);
}

TEST(EvaluationServiceTest, RejectsCorrectionAboveMaximum) {
    ServiceFixture fixture;
    fixture.seed();

    EXPECT_THROW((void)fixture.facade->correct_evaluation("S-101::homework", 11),
                 ValidationError);
}

TEST(EvaluationServiceTest, CancelsEvaluation) {
    ServiceFixture fixture;
    fixture.seed();

    fixture.facade->cancel_evaluation("S-101::exam");
    const auto evaluations = fixture.facade->list_evaluations("S-101");
    EXPECT_EQ(evaluations.size(), 2U);
    EXPECT_FALSE(std::any_of(evaluations.begin(), evaluations.end(),
                             [](const auto& evaluation) {
                                 return evaluation.criterion_id == "exam";
                             }));
}

TEST(EvaluationServiceTest, RejectsCancelOfUnknownEvaluation) {
    ServiceFixture fixture;
    fixture.seed();

    EXPECT_THROW(fixture.facade->cancel_evaluation("S-101::unknown"), NotFoundError);
}

}  // namespace
