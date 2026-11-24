#include <exception>
#include <iostream>

#include "app/cli.hpp"
#include "app/composition_root.hpp"
#include "app/demo_scenario.hpp"
#include "presentation/console/console_app.hpp"

int main(int argc, char** argv) {
    try {
        const auto cli = pos::app::parse_cli(argc, argv);
        if (cli.help) {
            std::cout << pos::app::cli_help();
            return 0;
        }

        auto facade = pos::app::build_facade(cli.config);

        if (cli.config.demo) {
            pos::app::DemoScenario demo{facade, std::cout};
            demo.run(cli.config.out_dir);
            return 0;
        }

        pos::presentation::ConsoleApp app{facade, cli.config.out_dir, std::cin, std::cout};
        app.run();
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Ошибка: " << error.what() << '\n';
        return 1;
    }
}
