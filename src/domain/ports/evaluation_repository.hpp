#pragma once

#include <optional>
#include <vector>

#include "domain/model/criterion_id.hpp"
#include "domain/model/evaluation.hpp"
#include "domain/model/evaluation_id.hpp"
#include "domain/model/student_id.hpp"

namespace pos::domain {

class EvaluationReader {
public:
    virtual ~EvaluationReader() = default;

    [[nodiscard]] virtual std::optional<Evaluation> find_by_id(
        const EvaluationId& id) const = 0;
    [[nodiscard]] virtual std::vector<Evaluation> find_by_student(
        const StudentId& student_id) const = 0;
    [[nodiscard]] virtual std::optional<Evaluation> find_by_student_and_criterion(
        const StudentId& student_id,
        const CriterionId& criterion_id) const = 0;
    [[nodiscard]] virtual std::vector<Evaluation> find_all() const = 0;
};

class EvaluationWriter {
public:
    virtual ~EvaluationWriter() = default;

    virtual void save(const Evaluation& evaluation) = 0;
    virtual void remove(const EvaluationId& id) = 0;
};

}  // namespace pos::domain
