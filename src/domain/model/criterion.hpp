#pragma once

#include <string>
#include <utility>

#include "domain/model/criterion_id.hpp"
#include "domain/model/max_score.hpp"
#include "domain/model/validation.hpp"
#include "domain/model/weight.hpp"

namespace pos::domain {

/// Критерий рубрикатора
class Criterion {
public:
    Criterion(CriterionId id, std::string name, MaxScore max_score, Weight weight)
        : id_(std::move(id)),
          name_(detail::trim_copy(name)),
          max_score_(max_score),
          weight_(weight) {
        detail::require_text(name_, "Название критерия", 100);
    }

    [[nodiscard]] const CriterionId& id() const noexcept { return id_; }
    [[nodiscard]] const std::string& name() const noexcept { return name_; }
    [[nodiscard]] const MaxScore& max_score() const noexcept { return max_score_; }
    [[nodiscard]] const Weight& weight() const noexcept { return weight_; }

private:
    CriterionId id_;
    std::string name_;
    MaxScore max_score_;
    Weight weight_;
};

}  // namespace pos::domain
