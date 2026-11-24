#include <gtest/gtest.h>

#include <chrono>
#include <optional>
#include <string>

#include "domain/errors.hpp"
#include "domain/model/criterion.hpp"
#include "domain/model/rubricator.hpp"
#include "domain/model/student.hpp"
#include "domain/services/evaluation_factory.hpp"

namespace {

using namespace pos::domain;

constexpr std::chrono::year_month_day date{std::chrono::year{2026},
                                           std::chrono::month{2},
                                           std::chrono::day{1}};

Student make_student() {
    return Student{StudentId{"S-101"}, FullName{"Иванов Иван"}, GroupName{"Б05-111"}};
}

Rubricator make_rubricator() {
    Rubricator rubricator;
    rubricator.add_criterion(
        Criterion{CriterionId{"exam"}, "Экзамен", MaxScore{10}, Weight{30.0}});
    return rubricator;
}

TEST(EvaluationTest, FactoryCreatesEvaluationWithCompositeIdentity) {
    const auto student = make_student();
    const auto rubricator = make_rubricator();

    const auto evaluation = EvaluationFactory::create(
        student, rubricator, CriterionId{"exam"}, 9, date, "  хорошая работа  ");

    EXPECT_EQ(evaluation.id().value(), "S-101::exam");
    EXPECT_EQ(evaluation.student_id(), student.id());
    EXPECT_EQ(evaluation.criterion_id(), CriterionId{"exam"});
    EXPECT_EQ(evaluation.score().points(), 9);
    EXPECT_EQ(evaluation.score().max_points(), 10);
    EXPECT_EQ(evaluation.comment(), std::optional<std::string>{"хорошая работа"});
    EXPECT_EQ(evaluation.date(), date);
}

TEST(EvaluationTest, FactoryRejectsCriterionOutsideRubricator) {
    EXPECT_THROW((void)EvaluationFactory::create(
                     make_student(), make_rubricator(), CriterionId{"unknown"}, 5, date),
                 NotFoundError);
}

TEST(EvaluationTest, FactoryRejectsScoreAboveCriterionMaximum) {
    EXPECT_THROW((void)EvaluationFactory::create(
                     make_student(), make_rubricator(), CriterionId{"exam"}, 11, date),
                 ValidationError);
}

TEST(EvaluationTest, FactoryRejectsNegativeScoreAndInvalidDate) {
    EXPECT_THROW((void)EvaluationFactory::create(
                     make_student(), make_rubricator(), CriterionId{"exam"}, -1, date),
                 ValidationError);
    EXPECT_THROW((void)EvaluationFactory::create(
                     make_student(), make_rubricator(), CriterionId{"exam"}, 5,
                     std::chrono::year_month_day{std::chrono::year{2026},
                                                 std::chrono::month{13},
                                                 std::chrono::day{1}}),
                 ValidationError);
}

TEST(EvaluationTest, RescoreKeepsMaximumBound) {
    auto evaluation = EvaluationFactory::create(
        make_student(), make_rubricator(), CriterionId{"exam"}, 5, date);

    evaluation.rescore(10);
    EXPECT_EQ(evaluation.score().points(), 10);

    EXPECT_THROW(evaluation.rescore(11), ValidationError);
    EXPECT_THROW(evaluation.rescore(-1), ValidationError);
}

TEST(EvaluationTest, BlankCommentBecomesEmpty) {
    auto evaluation = EvaluationFactory::create(
        make_student(), make_rubricator(), CriterionId{"exam"}, 5, date, "   ");
    EXPECT_FALSE(evaluation.comment().has_value());
}

}  // namespace

