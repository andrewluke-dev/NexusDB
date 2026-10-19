
#include "Record.hpp"

#include <utility>

Record::Record(
    int id,
    std::string name,
    std::string value
)
    : id_(id),
      name_(std::move(name)),
      value_(std::move(value)) {}

int Record::getId() const {
    return id_;
}

const std::string& Record::getName() const {
    return name_;
}

const std::string& Record::getValue() const {
    return value_;
}

void Record::setName(const std::string& name) {
    name_ = name;
}

void Record::setValue(const std::string& value) {
    value_ = value;
}
