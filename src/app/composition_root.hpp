#pragma once

#include <memory>

#include "app/app_config.hpp"
#include "application/pos_facade.hpp"

namespace pos::app {

/// Использование Boost.DI (boost-ext.di) для разрешения зависимостей на этапе компиляции.
[[nodiscard]] std::shared_ptr<application::PosFacade> build_facade(
    const AppConfig& config);

}  // namespace pos::app
