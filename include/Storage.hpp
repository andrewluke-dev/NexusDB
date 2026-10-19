
#pragma once

#include "Database.hpp"

#include <string>

class Storage {
public:
    explicit Storage(std::string filename = "data/nexusdb.txt");

    bool save(const Database& database) const;
    bool load(Database& database) const;

private:
    std::string filename_;
};
