#include "app/cli.hpp"

#include <sstream>
#include <stdexcept>
#include <string>

#include <boost/program_options.hpp>

namespace pos::app {
namespace {

namespace po = boost::program_options;

po::options_description make_options(CliResult& result) {
    po::options_description description{
        "ПОС — преподавательская оценка студентов\n\nДопустимые опции"};
    description.add_options()
        ("help,h", po::bool_switch(&result.help), "показать эту справку")
        ("demo", po::bool_switch(&result.config.demo),
         "прогнать сквозной демонстрационный сценарий без интерактива")
        ("formula", po::value<std::string>(&result.config.formula)
                        ->default_value("weighted-average"),
         "формула итога: weighted-average | weighted-points")
        ("rounding", po::value<int>(&result.config.rounding)->default_value(1),
         "число знаков после запятой для итога (0..4)")
        ("out", po::value<std::string>()->default_value("out"),
         "каталог для экспорта CSV/JSON");
    return description;
}

}  // namespace

CliResult parse_cli(int argc, char** argv) {
    CliResult result;
    auto description = make_options(result);

    try {
        po::variables_map variables;
        po::store(po::parse_command_line(argc, argv, description), variables);
        po::notify(variables);
        result.config.out_dir = variables["out"].as<std::string>();
    } catch (const po::error& error) {
        throw std::runtime_error(std::string{"Ошибка аргументов командной строки: "} +
                                 error.what());
    }

    return result;
}

std::string cli_help() {
    CliResult dummy;
    const auto description = make_options(dummy);
    std::ostringstream out;
    out << "Использование: pos [опции]\n\n" << description << '\n'
        << "Без опций запускается интерактивное меню.\n"
        << "Пример: pos --demo --formula weighted-points --rounding 1 --out out\n";
    return out.str();
}

}  // namespace pos::app
