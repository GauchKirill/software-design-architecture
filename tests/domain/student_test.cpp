#include <gtest/gtest.h>

#include <string>

#include "domain/errors.hpp"
#include "domain/model/full_name.hpp"
#include "domain/model/group_name.hpp"
#include "domain/model/student.hpp"
#include "domain/model/student_id.hpp"

namespace {

using namespace pos::domain;

TEST(StudentTest, NormalizesTextValues) {
    const FullName name{"  Иванов Иван  "};
    const GroupName group{"  Б05-111 "};
    EXPECT_EQ(name.value(), "Иванов Иван");
    EXPECT_EQ(group.value(), "Б05-111");
}

TEST(StudentTest, RejectsEmptyNameAndGroup) {
    EXPECT_THROW((void)FullName{"   "}, ValidationError);
    EXPECT_THROW((void)GroupName{""}, ValidationError);
}

TEST(StudentTest, IdentifierRejectsWhitespaceAndColon) {
    EXPECT_THROW((void)StudentId{"S 101"}, ValidationError);
    EXPECT_THROW((void)StudentId{"S:101"}, ValidationError);
    EXPECT_THROW((void)StudentId{""}, ValidationError);
}

TEST(StudentTest, KeepsIdentityWhenAttributesChange) {
    Student student{StudentId{"S-101"}, FullName{"Иванов Иван"}, GroupName{"Б05-111"}};
    const auto id = student.id();

    student.change_name(FullName{"Иванов И. И."});
    student.change_group(GroupName{"Б05-112"});

    EXPECT_EQ(student.id(), id);
    EXPECT_EQ(student.full_name().value(), "Иванов И. И.");
    EXPECT_EQ(student.group().value(), "Б05-112");
}

}  // namespace
