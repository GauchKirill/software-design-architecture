#pragma once

#include <optional>
#include <vector>

#include "domain/model/student.hpp"
#include "domain/model/student_id.hpp"

namespace pos::domain {

/// Отдельные интерфейсы чтения и записи (ISP): сценарий, которому нужно
/// только читать, не получает доступ к записи.
class StudentReader {
public:
    virtual ~StudentReader() = default;

    [[nodiscard]] virtual std::optional<Student> find_by_id(const StudentId& id) const = 0;
    [[nodiscard]] virtual std::vector<Student> find_all() const = 0;
    [[nodiscard]] virtual bool exists(const StudentId& id) const = 0;
};

class StudentWriter {
public:
    virtual ~StudentWriter() = default;

    virtual void save(const Student& student) = 0;
    virtual void remove(const StudentId& id) = 0;
};

}  // namespace pos::domain
