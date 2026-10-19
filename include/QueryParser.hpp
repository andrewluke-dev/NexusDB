
#pragma once

#include <string>

enum class QueryType {
    Insert,
    Select,
    SelectAll,
    Update,
    Delete,
    Help,
    Exit,
    Invalid
};

struct Query {
    QueryType type = QueryType::Invalid;
    int id = 0;
    std::string name;
    std::string value;
    std::string error;
};

class QueryParser {
public:
    static Query parse(const std::string& input);
};
