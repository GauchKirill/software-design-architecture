#include "application/services/rubricator_service.hpp"

#include <string>
#include <utility>

#include "application/mapping.hpp"
#include "domain/model/criterion.hpp"
#include "domain/model/max_score.hpp"
#include "domain/model/weight.hpp"

namespace pos::application {

RubricatorService::RubricatorService(std::shared_ptr<domain::RubricatorReader> reader,
                                     std::shared_ptr<domain::RubricatorWriter> writer)
    : reader_(std::move(reader)), writer_(std::move(writer)) {}

CriterionDto RubricatorService::add_criterion(std::string_view id,
                                              std::string_view name,
                                              int max_score,
                                              double weight) {
    domain::Criterion criterion{
        domain::CriterionId{std::string{id}},
        std::string{name},
        domain::MaxScore{max_score},
        domain::Weight{weight},
    };

    auto rubricator = reader_->load();
    rubricator.add_criterion(criterion);
    writer_->save(rubricator);
    return to_dto(criterion);
}

std::vector<CriterionDto> RubricatorService::list_criteria() const {
    std::vector<CriterionDto> result;
    for (const auto& criterion : reader_->load().criteria()) {
        result.push_back(to_dto(criterion));
    }
    return result;
}

}  // namespace pos::application
