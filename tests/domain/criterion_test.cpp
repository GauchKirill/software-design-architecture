#include <gtest/gtest.h>

#include "domain/errors.hpp"
#include "domain/model/criterion.hpp"
#include "domain/model/criterion_id.hpp"
#include "domain/model/max_score.hpp"
#include "domain/model/weight.hpp"

namespace {

using namespace pos::domain;

TEST(CriterionTest, CreatesValidCriterionAndTrimsName) {
    const Criterion criterion{CriterionId{"exam"}, "  Экзамен  ", MaxScore{10}, Weight{30.0}};
    EXPECT_EQ(criterion.id().value(), "exam");
    EXPECT_EQ(criterion.name(), "Экзамен");
    EXPECT_EQ(criterion.max_score().value(), 10);
    EXPECT_DOUBLE_EQ(criterion.weight().value(), 30.0);
}

TEST(CriterionTest, RejectsInvalidMaximum) {
    EXPECT_THROW((void)MaxScore{0}, ValidationError);
    EXPECT_THROW((void)MaxScore{-1}, ValidationError);
    EXPECT_THROW((void)MaxScore{1001}, ValidationError);
}

TEST(CriterionTest, RejectsInvalidWeight) {
    EXPECT_THROW((void)Weight{0.0}, ValidationError);
    EXPECT_THROW((void)Weight{-1.0}, ValidationError);
    EXPECT_THROW((void)Weight{1001.0}, ValidationError);
}

TEST(CriterionTest, RejectsEmptyName) {
    EXPECT_THROW((void)(Criterion{CriterionId{"exam"}, "   ", MaxScore{10}, Weight{1.0}}),
                 ValidationError);
}

}  // namespace
