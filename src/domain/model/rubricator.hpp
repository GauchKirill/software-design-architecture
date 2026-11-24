#pragma once

#include <cstddef>
#include <vector>

#include "domain/model/criterion.hpp"

namespace pos::domain {

/// Рубрикатор — агрегат-корень, который хранит критерии и охраняет их уникальность.
/// Инвариант «оценка только по критерию из рубрикатора» обеспечивается тем, что
/// EvaluationFactory принимает Criterion именно отсюда, а не произвольный объект.
class Rubricator {
public:
    void add_criterion(Criterion criterion);

    [[nodiscard]] const Criterion* find(const CriterionId& id) const noexcept;
    [[nodiscard]] bool contains(const CriterionId& id) const noexcept;
    [[nodiscard]] const std::vector<Criterion>& criteria() const noexcept { return criteria_; }
    [[nodiscard]] std::size_t size() const noexcept { return criteria_.size(); }
    [[nodiscard]] bool empty() const noexcept { return criteria_.empty(); }
    [[nodiscard]] double total_weight() const noexcept;

private:
    std::vector<Criterion> criteria_;
};

}  // namespace pos::domain
