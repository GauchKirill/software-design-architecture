#include <gtest/gtest.h>

#include "domain/errors.hpp"
#include "domain/model/score.hpp"

namespace {

using pos::domain::Score;
using pos::domain::ValidationError;

TEST(ScoreTest, AcceptsBoundaryValues) {
    const auto zero = Score::of(0, 10);
    EXPECT_EQ(zero.points(), 0);
    EXPECT_EQ(zero.max_points(), 10);

    const auto maximum = Score::of(10, 10);
    EXPECT_EQ(maximum.points(), 10);
}

TEST(ScoreTest, RejectsNegativePoints) {
    EXPECT_THROW((void)Score::of(-1, 10), ValidationError);
}

TEST(ScoreTest, RejectsPointsAboveMaximum) {
    EXPECT_THROW((void)Score::of(11, 10), ValidationError);
}

TEST(ScoreTest, RejectsNonPositiveMaximum) {
    EXPECT_THROW((void)Score::of(0, 0), ValidationError);
}

TEST(ScoreTest, ComputesRatio) {
    EXPECT_DOUBLE_EQ(Score::of(5, 10).ratio(), 0.5);
    EXPECT_DOUBLE_EQ(Score::of(0, 3).ratio(), 0.0);
}

}  // namespace
