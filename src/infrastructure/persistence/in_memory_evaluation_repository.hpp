#pragma once

#include <memory>
#include <optional>
#include <utility>
#include <vector>

#include "domain/ports/evaluation_repository.hpp"
#include "infrastructure/persistence/in_memory_database.hpp"

namespace pos::infrastructure {

class InMemoryEvaluationRepository final : public domain::EvaluationReader,
                                           public domain::EvaluationWriter {
public:
    explicit InMemoryEvaluationRepository(std::shared_ptr<InMemoryDatabase> database)
        : database_(std::move(database)) {}

    [[nodiscard]] std::optional<domain::Evaluation> find_by_id(
        const domain::EvaluationId& id) const override {
        const auto it = database_->evaluations.find(id.value());
        if (it == database_->evaluations.end()) {
            return std::nullopt;
        }
        return it->second;
    }

    [[nodiscard]] std::vector<domain::Evaluation> find_by_student(
        const domain::StudentId& student_id) const override {
        std::vector<domain::Evaluation> result;
        for (const auto& [key, evaluation] : database_->evaluations) {
            (void)key;
            if (evaluation.student_id() == student_id) {
                result.push_back(evaluation);
            }
        }
        return result;
    }

    [[nodiscard]] std::optional<domain::Evaluation> find_by_student_and_criterion(
        const domain::StudentId& student_id,
        const domain::CriterionId& criterion_id) const override {
        for (const auto& [key, evaluation] : database_->evaluations) {
            (void)key;
            if (evaluation.student_id() == student_id &&
                evaluation.criterion_id() == criterion_id) {
                return evaluation;
            }
        }
        return std::nullopt;
    }

    [[nodiscard]] std::vector<domain::Evaluation> find_all() const override {
        std::vector<domain::Evaluation> result;
        result.reserve(database_->evaluations.size());
        for (const auto& [key, evaluation] : database_->evaluations) {
            (void)key;
            result.push_back(evaluation);
        }
        return result;
    }

    void save(const domain::Evaluation& evaluation) override {
        database_->evaluations.insert_or_assign(evaluation.id().value(), evaluation);
    }

    void remove(const domain::EvaluationId& id) override {
        database_->evaluations.erase(id.value());
    }

private:
    std::shared_ptr<InMemoryDatabase> database_;
};

}  // namespace pos::infrastructure
