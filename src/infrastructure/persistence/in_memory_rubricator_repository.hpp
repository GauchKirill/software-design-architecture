#pragma once

#include <memory>
#include <utility>

#include "domain/ports/rubricator_repository.hpp"
#include "infrastructure/persistence/in_memory_database.hpp"

namespace pos::infrastructure {

class InMemoryRubricatorRepository final : public domain::RubricatorReader,
                                           public domain::RubricatorWriter {
public:
    explicit InMemoryRubricatorRepository(std::shared_ptr<InMemoryDatabase> database)
        : database_(std::move(database)) {}

    [[nodiscard]] domain::Rubricator load() const override {
        return database_->rubricator;
    }

    void save(const domain::Rubricator& rubricator) override {
        database_->rubricator = rubricator;
    }

private:
    std::shared_ptr<InMemoryDatabase> database_;
};

}  // namespace pos::infrastructure
