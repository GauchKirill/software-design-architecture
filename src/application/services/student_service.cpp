#include "application/services/student_service.hpp"

#include <string>
#include <utility>

#include "application/mapping.hpp"
#include "domain/errors.hpp"
#include "domain/model/full_name.hpp"
#include "domain/model/group_name.hpp"
#include "domain/model/student.hpp"
#include "domain/model/student_id.hpp"

namespace pos::application {

StudentService::StudentService(std::shared_ptr<domain::StudentReader> reader,
                               std::shared_ptr<domain::StudentWriter> writer)
    : reader_(std::move(reader)), writer_(std::move(writer)) {}

StudentDto StudentService::add_student(std::string_view id,
                                       std::string_view full_name,
                                       std::string_view group) {
    domain::Student student{
        domain::StudentId{std::string{id}},
        domain::FullName{std::string{full_name}},
        domain::GroupName{std::string{group}},
    };

    if (reader_->exists(student.id())) {
        throw domain::ConflictError("Студент с номером зачётки '" + student.id().value() +
                                    "' уже существует");
    }

    writer_->save(student);
    return to_dto(student);
}

std::vector<StudentDto> StudentService::list_students() const {
    std::vector<StudentDto> result;
    for (const auto& student : reader_->find_all()) {
        result.push_back(to_dto(student));
    }
    return result;
}

}  // namespace pos::application
