
#include "Storage.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>

Storage::Storage(std::string filename)
    : filename_(std::move(filename)) {}

bool Storage::save(const Database& database) const {
    namespace fs = std::filesystem;

    const fs::path path(filename_);
    const fs::path parent = path.parent_path();

    std::error_code error;

    if (!parent.empty()) {
        fs::create_directories(parent, error);

        if (error) {
            return false;
        }
    }

    // Write to a temporary file first.
    const fs::path temporary = path.string() + ".tmp";

    std::ofstream output(temporary, std::ios::trunc);

    if (!output.is_open()) {
        return false;
    }

    for (const auto& record : database.selectAll()) {
        output << record.getId() << ' '
               << std::quoted(record.getName()) << ' '
               << std::quoted(record.getValue()) << '\n';

        if (!output) {
            output.close();
            fs::remove(temporary, error);
            return false;
        }
    }

    output.close();

    if (!output) {
        fs::remove(temporary, error);
        return false;
    }

    fs::rename(temporary, path, error);

    if (error) {
        fs::remove(temporary, error);
        return false;
    }

    return true;
}

bool Storage::load(Database& database) const {
    namespace fs = std::filesystem;

    std::error_code error;

    if (!fs::exists(filename_, error)) {
        return !error;
    }

    std::ifstream input(filename_);

    if (!input.is_open()) {
        return false;
    }

    Database loaded;
    std::string line;

    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        std::istringstream stream(line);

        int id = 0;
        std::string name;
        std::string value;
        std::string extra;

        if (!(stream >> id
                     >> std::quoted(name)
                     >> std::quoted(value))) {
            return false;
        }

        if (stream >> extra) {
            return false;
        }

        if (!loaded.insert(id, name, value)) {
            return false;
        }
    }

    if (input.bad()) {
        return false;
    }

    database = std::move(loaded);
    return true;
}
