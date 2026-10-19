
#include "Database.hpp"

#include <algorithm>
#include <utility>

bool Database::insert(
    int id,
    const std::string& name,
    const std::string& value
) {
    if (id <= 0 || name.empty()) {
        return false;
    }

    auto result = records_.emplace(
        id,
        Record(id, name, value)
    );

    return result.second;
}

std::optional<Record> Database::select(int id) const {
    auto it = records_.find(id);

    if (it == records_.end()) {
        return std::nullopt;
    }

    return it->second;
}

bool Database::update(
    int id,
    const std::string& name,
    const std::string& value
) {
    if (name.empty()) {
        return false;
    }

    auto it = records_.find(id);

    if (it == records_.end()) {
        return false;
    }

    it->second.setName(name);
    it->second.setValue(value);

    return true;
}

bool Database::remove(int id) {
    return records_.erase(id) > 0;
}

std::vector<Record> Database::selectAll() const {
    std::vector<Record> result;
    result.reserve(records_.size());

    for (const auto& entry : records_) {
        result.push_back(entry.second);
    }

    std::sort(
        result.begin(),
        result.end(),
        [](const Record& a, const Record& b) {
            return a.getId() < b.getId();
        }
    );

    return result;
}

bool Database::contains(int id) const {
    return records_.find(id) != records_.end();
}

std::size_t Database::size() const {
    return records_.size();
}

bool Database::empty() const {
    return records_.empty();
}

void Database::clear() {
    records_.clear();
}
