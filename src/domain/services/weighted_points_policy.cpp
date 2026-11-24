#include "domain/services/weighted_points_policy.hpp"

#include "domain/errors.hpp"

namespace pos::domain {

FinalScore WeightedPointsPolicy::calculate(
    const Rubricator& rubricator,
    std::span<const Evaluation> evaluations) const {
    if (rubricator.empty()) {
        throw ValidationError("Нельзя посчитать итог по пустому рубрикатору");
    }

    double weighted_points = 0.0;
    double weighted_maximum = 0.0;

    for (const auto& criterion : rubricator.criteria()) {
        double points = 0.0;
        for (const auto& evaluation : evaluations) {
            if (evaluation.criterion_id() == criterion.id()) {
                points = static_cast<double>(evaluation.score().points());
                break;
            }
        }
        weighted_points += points * criterion.weight().value();
        weighted_maximum += static_cast<double>(criterion.max_score().value()) *
                            criterion.weight().value();
    }

    if (weighted_maximum <= 0.0) {
        throw ValidationError("Взвешенный максимум должен быть положительным");
    }

    return FinalScore::percent(weighted_points / weighted_maximum * 100.0);
}

}  // namespace pos::domain
