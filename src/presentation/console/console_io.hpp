#pragma once

#include <istream>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>

namespace pos::presentation {

/// Ввод/вывод консоли вынесен из сценариев: ConsoleApp можно тестировать
/// с подменёнными потоками, а парсинг не размазан по меню.
class ConsoleIo {
public:
    ConsoleIo(std::istream& in, std::ostream& out);

    [[nodiscard]] std::string read_line(std::string_view prompt);
    [[nodiscard]] int read_int(std::string_view prompt);
    [[nodiscard]] double read_double(std::string_view prompt);
    [[nodiscard]] std::optional<std::string> read_optional(std::string_view prompt);
    [[nodiscard]] bool eof() const noexcept;

private:
    std::istream& in_;
    std::ostream& out_;
};

}  // namespace pos::presentation
