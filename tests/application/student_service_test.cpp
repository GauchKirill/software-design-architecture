#include <gtest/gtest.h>

#include <string>

#include "domain/errors.hpp"
#include "support/service_fixture.hpp"

namespace {

using pos::domain::ConflictError;
using pos::domain::ValidationError;
using pos::test::ServiceFixture;

TEST(StudentServiceTest, AddsAndListsStudents) {
    ServiceFixture fixture;

    const auto student = fixture.facade->add_student("S-101", "  Иванов Иван ", " Б05-111 ");
    EXPECT_EQ(student.id, "S-101");
    EXPECT_EQ(student.full_name, "Иванов Иван");
    EXPECT_EQ(student.group, "Б05-111");

    const auto students = fixture.facade->list_students();
    ASSERT_EQ(students.size(), 1U);
    EXPECT_EQ(students.front().id, "S-101");
}

TEST(StudentServiceTest, RejectsDuplicateStudent) {
    ServiceFixture fixture;
    (void)fixture.facade->add_student("S-101", "Иванов Иван", "Б05-111");

    EXPECT_THROW((void)fixture.facade->add_student("S-101", "Петров Пётр", "Б05-112"),
                 ConflictError);
}

TEST(StudentServiceTest, RejectsInvalidStudentData) {
    ServiceFixture fixture;
    EXPECT_THROW((void)fixture.facade->add_student("S 101", "Иванов Иван", "Б05-111"),
                 ValidationError);
    EXPECT_THROW((void)fixture.facade->add_student("S-101", "", "Б05-111"), ValidationError);
    EXPECT_THROW((void)fixture.facade->add_student("S-101", "Иванов Иван", ""),
                 ValidationError);
}

}  // namespace
