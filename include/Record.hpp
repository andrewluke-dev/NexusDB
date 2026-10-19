
#pragma once

#include <string>

class Record {
public:
    Record(int id, std::string name, std::string value);

    int getId() const;
    const std::string& getName() const;
    const std::string& getValue() const;

    void setName(const std::string& name);
    void setValue(const std::string& value);

private:
    int id_;
    std::string name_;
    std::string value_;
};
