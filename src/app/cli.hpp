#pragma once

#include <string>

#include "app/app_config.hpp"

namespace pos::app {

struct CliResult {
    AppConfig config;
    bool help{false};
};

/// Разбор аргументов командной строки
[[nodiscard]] CliResult parse_cli(int argc, char** argv);

[[nodiscard]] std::string cli_help();

}  // namespace pos::app
