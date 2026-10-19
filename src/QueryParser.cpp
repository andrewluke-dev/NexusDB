
#include "QueryParser.hpp"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <limits>
#include <sstream>
#include <string>

namespace {

std::string uppercase(std::string text) {
    std::transform(
        text.begin(),
        text.end(),
        text.begin(),
        [](unsigned char c) {
            return static_cast<char>(std::toupper(c));
        }
    );

    return text;
}

bool readId(std::istringstream& stream, int& id) {
    long long number = 0;

    if (!(stream >> number)) {
        return false;
    }

    if (number <= 0 ||
        number > std::numeric_limits<int>::max()) {
        return false;
    }

    id = static_cast<int>(number);
    return true;
}

bool noExtraArguments(std::istringstream& stream) {
    std::string extra;
    return !(stream >> extra);
}

Query invalid(const std::string& message) {
    Query result;
    result.type = QueryType::Invalid;
    result.error = message;
    return result;
}

} // namespace

Query QueryParser::parse(const std::string& input) {
    std::istringstream stream(input);

    std::string command;

    if (!(stream >> command)) {
        return invalid("Empty command.");
    }

    command = uppercase(command);

    Query query;

    if (command == "HELP") {
        if (!noExtraArguments(stream)) {
            return invalid("Usage: HELP");
        }

        query.type = QueryType::Help;
        return query;
    }

    if (command == "EXIT" || command == "QUIT") {
        if (!noExtraArguments(stream)) {
            return invalid("Usage: EXIT");
        }

        query.type = QueryType::Exit;
        return query;
    }

    if (command == "SELECT") {
        std::string argument;

        if (!(stream >> argument)) {
            return invalid("Usage: SELECT <id> or SELECT ALL");
        }

        if (uppercase(argument) == "ALL") {
            if (!noExtraArguments(stream)) {
                return invalid("Usage: SELECT ALL");
            }

            query.type = QueryType::SelectAll;
            return query;
        }

        std::istringstream idStream(argument);

        if (!readId(idStream, query.id) ||
            !noExtraArguments(idStream) ||
            !noExtraArguments(stream)) {
            return invalid("Invalid SELECT ID.");
        }

        query.type = QueryType::Select;
        return query;
    }

    if (command == "DELETE") {
        if (!readId(stream, query.id) ||
            !noExtraArguments(stream)) {
            return invalid("Usage: DELETE <id>");
        }

        query.type = QueryType::Delete;
        return query;
    }

    if (command == "INSERT" || command == "UPDATE") {
        if (!readId(stream, query.id)) {
            return invalid("Invalid record ID.");
        }

        if (!(stream >> std::quoted(query.name)) ||
            !(stream >> std::quoted(query.value)) ||
            !noExtraArguments(stream)) {
            return invalid(
                "Usage: INSERT/UPDATE <id> "
                "\"name\" \"value\""
            );
        }

        if (query.name.empty()) {
            return invalid("Record name cannot be empty.");
        }

        query.type = command == "INSERT"
            ? QueryType::Insert
            : QueryType::Update;

        return query;
    }

    return invalid("Unknown command. Type HELP.");
}
