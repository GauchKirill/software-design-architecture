#pragma once

#include <chrono>

namespace pos::domain {

/// Порт времени: домен не зависит от системных часов и от тестов.
class Clock {
public:
    virtual ~Clock() = default;
    [[nodiscard]] virtual std::chrono::year_month_day today() const = 0;
};

}  // namespace pos::domain
