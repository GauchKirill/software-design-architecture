#include <gtest/gtest.h>

#include <string>

#include "domain/errors.hpp"
#include "domain/model/criterion.hpp"
#include "domain/model/rubricator.hpp"

namespace {

using namespace pos::domain;

Criterion make_criterion(std::string id, std::string name, int max, double weight) {
    return Criterion{CriterionId{std::move(id)}, std::move(name), MaxScore{max},
                     Weight{weight}};
}

TEST(RubricatorTest, AddsAndFindsCriteria) {
    Rubricator rubricator;
    rubricator.add_criterion(make_criterion("attendance", "Посещаемость", 5, 20.0));
    rubricator.add_criterion(make_criterion("exam", "Экзамен", 10, 30.0));

    ASSERT_EQ(rubricator.size(), 2U);
    ASSERT_NE(rubricator.find(CriterionId{"exam"}), nullptr);
    EXPECT_EQ(rubricator.find(CriterionId{"exam"})->name(), "Экзамен");
    EXPECT_TRUE(rubricator.contains(CriterionId{"attendance"}));
    EXPECT_FALSE(rubricator.contains(CriterionId{"homework"}));
    EXPECT_DOUBLE_EQ(rubricator.total_weight(), 50.0);
}

TEST(RubricatorTest, RejectsDuplicateId) {
    Rubricator rubricator;
    rubricator.add_criterion(make_criterion("exam", "Экзамен", 10, 30.0));
    EXPECT_THROW(rubricator.add_criterion(make_criterion("exam", "Другой экзамен", 5, 10.0)),
                 ConflictError);
}

TEST(RubricatorTest, RejectsDuplicateName) {
    Rubricator rubricator;
    rubricator.add_criterion(make_criterion("exam", "Экзамен", 10, 30.0));
    EXPECT_THROW(rubricator.add_criterion(make_criterion("exam2", "Экзамен", 5, 10.0)),
                 ConflictError);
}

}  // namespace
