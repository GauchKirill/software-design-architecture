#pragma once

#include <chrono>
#include <optional>

#include "domain/model/criterion_id.hpp"
#include "domain/model/evaluation.hpp"
#include "domain/model/rubricator.hpp"
#include "domain/model/student.hpp"

namespace pos::domain {

/// Порождающий паттерн Factory Method: единственная точка создания Evaluation.
/// Фабрика принимает Rubricator и CriterionId, сама находит критерий и проверяет,
/// что балл входит в его диапазон. Поэтому оценка по «критерию, которого нет
/// в рубрикаторе», невыразима даже при прямом вызове фабрики.
class EvaluationFactory {
public:
    [[nodiscard]] static Evaluation create(const Student& student,
                                           const Rubricator& rubricator,
                                           const CriterionId& criterion_id,
                                           int points,
                                           std::chrono::year_month_day date,
                                           std::optional<std::string> comment = std::nullopt);
};

}  // namespace pos::domain

