#include "domain/services/weighted_average_policy.hpp"

#include "domain/errors.hpp"

namespace pos::domain {

FinalScore WeightedAveragePolicy::calculate(
    const Rubricator& rubricator,
    std::span<const Evaluation> evaluations) const {
    if (rubricator.empty()) {
        throw ValidationError("Нельзя посчитать итог по пустому рубрикатору");
    }

    double weighted_ratio = 0.0;
    double total_weight = 0.0;

    for (const auto& criterion : rubricator.criteria()) {
        double points = 0.0;
        for (const auto& evaluation : evaluations) {
            if (evaluation.criterion_id() == criterion.id()) {
                points = static_cast<double>(evaluation.score().points());
                break;
            }
        }
        weighted_ratio += criterion.weight().value() *
                          (points / static_cast<double>(criterion.max_score().value()));
        total_weight += criterion.weight().value();
    }

    if (total_weight <= 0.0) {
        throw ValidationError("Суммарный вес критериев должен быть положительным");
    }

    return FinalScore::percent(weighted_ratio / total_weight * 100.0);
}

}  // namespace pos::domain
