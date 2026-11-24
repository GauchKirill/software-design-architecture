#include "app/app_config.hpp"

#include "application/errors.hpp"

namespace pos::app {

void validate_app_config(const AppConfig& config) {
    if (config.formula != "weighted-average" && config.formula != "weighted-points") {
        throw application::ApplicationError(
            "Неизвестная формула итога '" + config.formula +
            "'. Допустимо: weighted-average, weighted-points");
    }
    if (config.rounding < 0 || config.rounding > 4) {
        throw application::ApplicationError(
            "Число знаков округления должно быть в диапазоне 0..4");
    }
}

}  // namespace pos::app
