#include "presentation/console/console_io.hpp"

#include <charconv>
#include <stdexcept>
#include <string>
#include <system_error>

#include <boost/algorithm/string.hpp>

namespace pos::presentation {

ConsoleIo::ConsoleIo(std::istream& in, std::ostream& out) : in_(in), out_(out) {}

std::string ConsoleIo::read_line(std::string_view prompt) {
    out_ << prompt;
    out_.flush();
    std::string line;
    if (!std::getline(in_, line)) {
        return {};
    }
    return boost::algorithm::trim_copy(line);
}

int ConsoleIo::read_int(std::string_view prompt) {
    const std::string text = read_line(prompt);
    int value = 0;
    const auto* begin = text.data();
    const auto* end = text.data() + text.size();
    const auto [ptr, error] = std::from_chars(begin, end, value);
    if (error != std::errc{} || ptr != end) {
        throw std::invalid_argument("Ожидается целое число, получено: '" + text + "'");
    }
    return value;
}

double ConsoleIo::read_double(std::string_view prompt) {
    const std::string text = read_line(prompt);
    double value = 0.0;
    const auto* begin = text.data();
    const auto* end = text.data() + text.size();
    const auto [ptr, error] = std::from_chars(begin, end, value);
    if (error != std::errc{} || ptr != end) {
        throw std::invalid_argument("Ожидается число, получено: '" + text + "'");
    }
    return value;
}

std::optional<std::string> ConsoleIo::read_optional(std::string_view prompt) {
    auto text = read_line(prompt);
    if (text.empty()) {
        return std::nullopt;
    }
    return text;
}

bool ConsoleIo::eof() const noexcept {
    return in_.eof();
}

}  // namespace pos::presentation
