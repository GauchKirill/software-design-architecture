#pragma once

#include <string>
#include <utility>

#include "domain/model/full_name.hpp"
#include "domain/model/group_name.hpp"
#include "domain/model/student_id.hpp"

namespace pos::domain {

/// Студент — Entity: идентичность задаётся номером зачётки и сохраняется,
/// даже если меняются ФИО или группа.
class Student {
public:
    Student(StudentId id, FullName full_name, GroupName group)
        : id_(std::move(id)), full_name_(std::move(full_name)), group_(std::move(group)) {}

    [[nodiscard]] const StudentId& id() const noexcept { return id_; }
    [[nodiscard]] const FullName& full_name() const noexcept { return full_name_; }
    [[nodiscard]] const GroupName& group() const noexcept { return group_; }

    void change_name(FullName full_name) { full_name_ = std::move(full_name); }
    void change_group(GroupName group) { group_ = std::move(group); }

private:
    StudentId id_;
    FullName full_name_;
    GroupName group_;
};

}  // namespace pos::domain
