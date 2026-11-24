#pragma once

#include <map>
#include <string>

#include "domain/model/evaluation.hpp"
#include "domain/model/rubricator.hpp"
#include "domain/model/student.hpp"

namespace pos::infrastructure {

/// Единственное изменяемое хранилище демо-приложения. Все адаптеры-репозитории
/// получают один и тот же экземпляр через DI, поэтому reader и writer видят
/// согласованное состояние.
class InMemoryDatabase {
public:
    InMemoryDatabase() = default;

    std::map<std::string, domain::Student> students;
    domain::Rubricator rubricator;
    std::map<std::string, domain::Evaluation> evaluations;
};

}  // namespace pos::infrastructure
