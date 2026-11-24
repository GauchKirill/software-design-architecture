#pragma once

#include <chrono>

#include "domain/ports/clock.hpp"

namespace pos::infrastructure {

class SystemClock final : public domain::Clock {
public:
    [[nodiscard]] std::chrono::year_month_day today() const override {
        const auto now = std::chrono::system_clock::now();
        const auto days = std::chrono::floor<std::chrono::days>(now);
        return std::chrono::year_month_day{days};
    }
};

}  // namespace pos::infrastructure
