#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <utility>

#include "domain/model/criterion_id.hpp"
#include "domain/model/evaluation_id.hpp"
#include "domain/model/score.hpp"
#include "domain/model/student_id.hpp"

namespace pos::domain {

class EvaluationFactory;

/// Оценка — отдельный агрегат-корень: «этому студенту по этому критерию
/// поставили такой балл». Создаётся только через EvaluationFactory,
/// поэтому балл всегда связан с реальным критерием рубрикатора.
class Evaluation {
public:
    [[nodiscard]] const EvaluationId& id() const noexcept { return id_; }
    [[nodiscard]] const StudentId& student_id() const noexcept { return student_id_; }
    [[nodiscard]] const CriterionId& criterion_id() const noexcept { return criterion_id_; }
    [[nodiscard]] const Score& score() const noexcept { return score_; }
    [[nodiscard]] std::chrono::year_month_day date() const noexcept { return date_; }
    [[nodiscard]] const std::optional<std::string>& comment() const noexcept { return comment_; }

    /// Исправление оценки. Максимум берётся из сохранённого Score,
    /// поэтому балл не может выйти за пределы критерия.
    void rescore(int points);
    void change_comment(std::optional<std::string> comment);

private:
    friend class EvaluationFactory;

    Evaluation(EvaluationId id,
               StudentId student_id,
               CriterionId criterion_id,
               Score score,
               std::chrono::year_month_day date,
               std::optional<std::string> comment)
        : id_(std::move(id)),
          student_id_(std::move(student_id)),
          criterion_id_(std::move(criterion_id)),
          score_(score),
          date_(date),
          comment_(std::move(comment)) {}

    EvaluationId id_;
    StudentId student_id_;
    CriterionId criterion_id_;
    Score score_;
    std::chrono::year_month_day date_;
    std::optional<std::string> comment_;
};

}  // namespace pos::domain
