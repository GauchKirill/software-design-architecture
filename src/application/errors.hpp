#pragma once

#include <stdexcept>
#include <string>

namespace pos::application {

/// Ошибка уровня приложения (неизвестный формат отчёта, ошибка конфигурации).
class ApplicationError : public std::runtime_error {
public:
    explicit ApplicationError(const std::string& message) : std::runtime_error(message) {}
};

}  // namespace pos::application
