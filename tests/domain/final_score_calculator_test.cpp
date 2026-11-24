#include <gtest/gtest.h>

#include <chrono>
#include <memory>
#include <vector>

#include "domain/errors.hpp"
#include "domain/model/rubricator.hpp"
#include "domain/model/student.hpp"
#include "domain/services/evaluation_factory.hpp"
#include "domain/services/final_score_calculator.hpp"
#include "domain/services/weighted_average_policy.hpp"

namespace {

using namespace pos::domain;

constexpr std::chrono::year_month_day date{std::chrono::year{2026},
                                           std::chrono::month{2},
                                           std::chrono::day{1}};

Student make_student(std::string id) {
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

FinalScoreCalculator make_calculator() {
    return FinalScoreCalculator{std::make_shared<WeightedAveragePolicy>()};
}

TEST(FinalScoreCalculatorTest, CalculatesResultAndCountsAssessedCriteria) {
    const auto rubricator = make_rubricator();
    const auto student = make_student("S-101");
    std::vector<Evaluation> evaluations;
    evaluations.push_back(EvaluationFactory::create(
        student, rubricator, CriterionId{"attendance"}, 4, date));
    evaluations.push_back(EvaluationFactory::create(
        student, rubricator, CriterionId{"homework"}, 8, date));
    evaluations.push_back(EvaluationFactory::create(
        student, rubricator, CriterionId{"exam"}, 9, date));

    const auto result = make_calculator().calculate(student.id(), rubricator, evaluations);
    EXPECT_NEAR(result.score.value(), 83.0, 1e-9);
    EXPECT_EQ(result.assessed_criteria, 3U);
    EXPECT_EQ(result.total_criteria, 3U);
}

TEST(FinalScoreCalculatorTest, RejectsEvaluationOfAnotherStudent) {
    const auto rubricator = make_rubricator();
    const auto student = make_student("S-101");
    const auto other = make_student("S-102");
    std::vector<Evaluation> evaluations;
    evaluations.push_back(EvaluationFactory::create(
        other, rubricator, CriterionId{"exam"}, 9, date));

    EXPECT_THROW((void)make_calculator().calculate(student.id(), rubricator, evaluations),
                 ValidationError);
}

TEST(FinalScoreCalculatorTest, RejectsCriterionOutsideRubricator) {
    const auto rubricator = make_rubricator();
    const auto student = make_student("S-101");
    const Criterion extra{CriterionId{"extra"}, "Дополнительно", MaxScore{5}, Weight{10.0}};
    auto extended = rubricator;
    extended.add_criterion(extra);
    std::vector<Evaluation> evaluations;
    evaluations.push_back(
        EvaluationFactory::create(student, extended, CriterionId{"extra"}, 5, date));

    EXPECT_THROW((void)make_calculator().calculate(student.id(), rubricator, evaluations),
                 ValidationError);
}

TEST(FinalScoreCalculatorTest, RejectsDuplicateEvaluationsForOneCriterion) {
    const auto rubricator = make_rubricator();
    const auto student = make_student("S-101");
    std::vector<Evaluation> evaluations;
    evaluations.push_back(EvaluationFactory::create(
        student, rubricator, CriterionId{"exam"}, 8, date));
    evaluations.push_back(EvaluationFactory::create(
        student, rubricator, CriterionId{"exam"}, 9, date));

    EXPECT_THROW((void)make_calculator().calculate(student.id(), rubricator, evaluations),
                 ConflictError);
}

TEST(FinalScoreCalculatorTest, RejectsEmptyRubricator) {
    const auto student = make_student("S-101");
    EXPECT_THROW((void)make_calculator().calculate(student.id(), Rubricator{}, {}),
                 ValidationError);
}

TEST(FinalScoreCalculatorTest, RejectsNullPolicy) {
    EXPECT_THROW((void)FinalScoreCalculator{nullptr}, ValidationError);
}

}  // namespace
