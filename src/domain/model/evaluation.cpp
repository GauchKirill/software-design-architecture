#include "domain/model/evaluation.hpp"

#include <utility>

#include "domain/model/validation.hpp"

namespace pos::domain {

void Evaluation::rescore(int points) {
    score_ = Score::of(points, score_.max_points());
}

void Evaluation::change_comment(std::optional<std::string> comment) {
    comment_ = detail::normalize_comment(std::move(comment));
}

}  // namespace pos::domain
