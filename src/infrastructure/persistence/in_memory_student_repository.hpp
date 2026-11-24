#pragma once

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "domain/ports/student_repository.hpp"
#include "infrastructure/persistence/in_memory_database.hpp"

namespace pos::infrastructure {

class InMemoryStudentRepository final : public domain::StudentReader,
                                        public domain::StudentWriter {
public:
    explicit InMemoryStudentRepository(std::shared_ptr<InMemoryDatabase> database)
        : database_(std::move(database)) {}

    [[nodiscard]] std::optional<domain::Student> find_by_id(
        const domain::StudentId& id) const override {
        const auto it = database_->students.find(id.value());
        if (it == database_->students.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    [[nodiscard]] std::vector<domain::Student> find_all() const override {
        std::vector<domain::Student> result;
        result.reserve(database_->students.size());
        for (const auto& [key, student] : database_->students) {
            (void)key;
            result.push_back(student);
        }
        return result;
    }

    [[nodiscard]] bool exists(const domain::StudentId& id) const override {
        return database_->students.contains(id.value());
    }

    void save(const domain::Student& student) override {
        database_->students.insert_or_assign(student.id().value(), student);
    }

    void remove(const domain::StudentId& id) override {
        database_->students.erase(id.value());
    }

private:
    std::shared_ptr<InMemoryDatabase> database_;
};

}  // namespace pos::infrastructure
