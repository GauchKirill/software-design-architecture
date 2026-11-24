#pragma once

#include <chrono>

#include "domain/ports/clock.hpp"

namespace pos::test {

class FixedClock final : public domain::Clock {
public:
    explicit FixedClock(std::chrono::year_month_day date) : date_(date) {}

    [[nodiscard]] std::chrono::year_month_day today() const override { return date_; }

private:
    std::chrono::year_month_day date_;
};

}  // namespace pos::test
