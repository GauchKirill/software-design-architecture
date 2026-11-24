#pragma once

#include "domain/model/rubricator.hpp"

namespace pos::domain {

/// Рубрикатор — агрегат-корень, поэтому репозиторий загружает/сохраняет его целиком.
class RubricatorReader {
public:
    virtual ~RubricatorReader() = default;
    [[nodiscard]] virtual Rubricator load() const = 0;
};

class RubricatorWriter {
public:
    virtual ~RubricatorWriter() = default;
    virtual void save(const Rubricator& rubricator) = 0;
};

}  // namespace pos::domain
