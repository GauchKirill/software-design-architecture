#include "infrastructure/reports/file_report_writer.hpp"

#include <ios>
#include <stdexcept>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace pos::infrastructure {

void FileReportWriter::write(const std::filesystem::path& path,
                             std::string_view content) const {
    const boost::filesystem::path target{path.string()};
    if (target.has_parent_path()) {
        boost::filesystem::create_directories(target.parent_path());
    }

    boost::filesystem::ofstream out{target, std::ios::binary};
    if (!out) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + target.string());
    }

    out.write(content.data(), static_cast<std::streamsize>(content.size()));
    if (!out) {
        throw std::runtime_error("Не удалось записать отчёт в файл: " + target.string());
    }
}

}  // namespace pos::infrastructure
