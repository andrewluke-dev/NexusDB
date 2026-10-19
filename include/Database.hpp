
#pragma once

#include "Record.hpp"

#include <cstddef>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class Database {
public:
    bool insert(
        int id,
        const std::string& name,
        const std::string& value
    );

    std::optional<Record> select(int id) const;

    bool update(
        int id,
        const std::string& name,
        const std::string& value
    );

    bool remove(int id);

    std::vector<Record> selectAll() const;

    bool contains(int id) const;
    std::size_t size() const;
    bool empty() const;

    void clear();

private:
    std::unordered_map<int, Record> records_;
};
