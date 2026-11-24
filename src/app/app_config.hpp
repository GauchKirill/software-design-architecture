#pragma once

#include <filesystem>
#include <string>

namespace pos::app {

struct AppConfig {
    bool demo{false};
    std::string formula{"weighted-average"};
    int rounding{1};
    std::filesystem::path out_dir{"out"};
};

/// Проверяет конфигурацию на этапе композиции
void validate_app_config(const AppConfig& config);

}  // namespace pos::app
