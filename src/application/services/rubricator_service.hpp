#pragma once

#include <memory>
#include <string_view>
#include <vector>

#include "application/dto/dtos.hpp"
#include "domain/ports/rubricator_repository.hpp"

namespace pos::application {

class RubricatorService {
public:
    RubricatorService(std::shared_ptr<domain::RubricatorReader> reader,
                      std::shared_ptr<domain::RubricatorWriter> writer);

    [[nodiscard]] CriterionDto add_criterion(std::string_view id,
                                              std::string_view name,
                                              int max_score,
                                              double weight);
    [[nodiscard]] std::vector<CriterionDto> list_criteria() const;

private:
    std::shared_ptr<domain::RubricatorReader> reader_;
    std::shared_ptr<domain::RubricatorWriter> writer_;
};

}  // namespace pos::application
