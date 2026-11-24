#pragma once

#include <filesystem>
#include <istream>
#include <memory>
#include <ostream>
#include <string_view>

#include "application/pos_facade.hpp"
#include "presentation/console/console_io.hpp"

namespace pos::presentation {

/// Консольный адаптер: только ввод/вывод и навигация по меню.
/// Все бизнес-сценарии делегируются в PosFacade.
class ConsoleApp {
public:
    ConsoleApp(std::shared_ptr<application::PosFacade> facade,
               std::filesystem::path out_dir,
               std::istream& in,
               std::ostream& out);

    void run();

private:
    void print_menu() const;
    void dispatch(std::string_view choice);
    void students_menu();
    void rubricator_menu();
    void evaluations_menu();
    void final_scores_menu();
    void export_menu();

    std::shared_ptr<application::PosFacade> facade_;
    std::filesystem::path out_dir_;
    std::ostream& out_;
    ConsoleIo io_;
};

}  // namespace pos::presentation
