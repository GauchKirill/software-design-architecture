#include "domain/model/rubricator.hpp"

#include <string>
#include <utility>

#include "domain/errors.hpp"

namespace pos::domain {

void Rubricator::add_criterion(Criterion criterion) {
    for (const auto& existing : criteria_) {
        if (existing.id() == criterion.id()) {
            throw ConflictError("Критерий с кодом '" + criterion.id().value() +
                                "' уже есть в рубрикаторе");
        }
        if (existing.name() == criterion.name()) {
            throw ConflictError("Критерий с названием '" + criterion.name() +
                                "' уже есть в рубрикаторе");
        }
    }
    criteria_.push_back(std::move(criterion));
}

const Criterion* Rubricator::find(const CriterionId& id) const noexcept {
    for (const auto& criterion : criteria_) {
        if (criterion.id() == id) {
            return &criterion;
        }
    }
    return nullptr;
}

bool Rubricator::contains(const CriterionId& id) const noexcept {
    return find(id) != nullptr;
}

double Rubricator::total_weight() const noexcept {
    double total = 0.0;
    for (const auto& criterion : criteria_) {
        total += criterion.weight().value();
    }
    return total;
}

}  // namespace pos::domain
