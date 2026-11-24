#pragma once

#include "application/pos_facade.hpp"

namespace pos::app {

/// Сквозной сценарий: студенты и критерии → оценки → отказ на 12/10 →
/// исправление → итог → экспорт CSV/JSON. Не зависит от консольного меню,
/// поэтому проверяется интеграционным тестом.
class DemoScenario {
public:
    DemoScenario(std::shared_ptr<application::PosFacade> facade, std::ostream& out);

    void run(const std::filesystem::path& out_dir) const;

private:
    std::shared_ptr<application::PosFacade> facade_;
    std::ostream& out_;
};

}  // namespace pos::app
