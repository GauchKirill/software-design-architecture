#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <vector>

#include "domain/errors.hpp"
#include "domain/model/criterion.hpp"
#include "domain/model/rubricator.hpp"
#include "domain/model/student.hpp"
#include "domain/services/evaluation_factory.hpp"
#include "domain/services/rounded_final_score_policy.hpp"
#include "domain/services/weighted_average_policy.hpp"
#include "domain/services/weighted_points_policy.hpp"

namespace {

using namespace pos::domain;

constexpr std::chrono::year_month_day date{std::chrono::year{2026},
                                           std::chrono::month{2},
                                           std::chrono::day{1}};

Student make_student(std::string id = "S-101") {
    return Student{StudentId{std::move(id)}, FullName{"Иванов Иван"}, GroupName{"Б05-111"}};
}

Rubricator make_rubricator() {
    Rubricator rubricator;
    rubricator.add_criterion(
        Criterion{CriterionId{"attendance"}, "Посещаемость", MaxScore{5}, Weight{20.0}});
    rubricator.add_criterion(
        Criterion{CriterionId{"homework"}, "Домашние задания", MaxScore{10}, Weight{50.0}});
    rubricator.add_criterion(
        Criterion{CriterionId{"exam"}, "Экзамен", MaxScore{10}, Weight{30.0}});
    return rubricator;
}

std::vector<Evaluation> make_evaluations(const Student& student,
                                         const Rubricator& rubricator,
                                         bool include_exam) {
    std::vector<Evaluation> evaluations;
    evaluations.push_back(EvaluationFactory::create(
        student, rubricator, CriterionId{"attendance"}, 4, date));
    evaluations.push_back(EvaluationFactory::create(
        student, rubricator, CriterionId{"homework"}, 8, date));
    if (include_exam) {
        evaluations.push_back(EvaluationFactory::create(
            student, rubricator, CriterionId{"exam"}, 9, date));
    }
    return evaluations;
}

TEST(FinalScorePolicyTest, WeightedAverageNormalizesEachCriterion) {
    const auto rubricator = make_rubricator();
    const auto student = make_student();
    const auto evaluations = make_evaluations(student, rubricator, true);

    const WeightedAveragePolicy policy;
    EXPECT_NEAR(policy.calculate(rubricator, evaluations).value(), 83.0, 1e-9);
}

TEST(FinalScorePolicyTest, WeightedPointsUsesAbsoluteWeightedPoints) {
    const auto rubricator = make_rubricator();
    const auto student = make_student();
    const auto evaluations = make_evaluations(student, rubricator, true);

    const WeightedPointsPolicy policy;
    EXPECT_NEAR(policy.calculate(rubricator, evaluations).value(), 83.3333333, 1e-6);
}

TEST(FinalScorePolicyTest, MissingEvaluationCountsAsZero) {
    const auto rubricator = make_rubricator();
    const auto student = make_student();
    const auto evaluations = make_evaluations(student, rubricator, false);

    const WeightedAveragePolicy average;
    EXPECT_NEAR(average.calculate(rubricator, evaluations).value(), 56.0, 1e-9);

    const WeightedPointsPolicy points;
    EXPECT_NEAR(points.calculate(rubricator, evaluations).value(), 53.3333333, 1e-6);
}

TEST(FinalScorePolicyTest, DecoratorRoundsResultAndKeepsContract) {
    const auto rubricator = make_rubricator();
    const auto student = make_student();
    const auto evaluations = make_evaluations(student, rubricator, true);

    const auto inner = std::make_shared<WeightedPointsPolicy>();
    const RoundedFinalScorePolicy one_decimal{inner, 1};
    const RoundedFinalScorePolicy zero_decimals{inner, 0};

    EXPECT_DOUBLE_EQ(one_decimal.calculate(rubricator, evaluations).value(), 83.3);
    EXPECT_DOUBLE_EQ(zero_decimals.calculate(rubricator, evaluations).value(), 83.0);

    // LSP: любой подтип FinalScorePolicy возвращает валидный FinalScore.
    for (const FinalScorePolicy* policy : {static_cast<const FinalScorePolicy*>(&one_decimal),
                                           static_cast<const FinalScorePolicy*>(&zero_decimals)}) {
        const auto score = policy->calculate(rubricator, evaluations);
        EXPECT_GE(score.value(), 0.0);
        EXPECT_LE(score.value(), 100.0);
    }
}

TEST(FinalScorePolicyTest, EmptyRubricatorIsRejected) {
    const Rubricator empty;
    const WeightedAveragePolicy average;
    EXPECT_THROW((void)average.calculate(empty, {}), ValidationError);
}

}  // namespace
