#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "application/dto/dtos.hpp"
#include "domain/ports/student_repository.hpp"

namespace pos::application {

class StudentService {
public:
    StudentService(std::shared_ptr<domain::StudentReader> reader,
                   std::shared_ptr<domain::StudentWriter> writer);

    [[nodiscard]] StudentDto add_student(std::string_view id,
                                         std::string_view full_name,
                                         std::string_view group);
    [[nodiscard]] std::vector<StudentDto> list_students() const;

private:
    std::shared_ptr<domain::StudentReader> reader_;
    std::shared_ptr<domain::StudentWriter> writer_;
};

}  // namespace pos::application
