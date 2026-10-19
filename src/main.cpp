
#include "Database.hpp"
#include "QueryParser.hpp"
#include "Storage.hpp"

#include <iostream>
#include <string>

void printHelp() {
    std::cout << "\n========== NEXUSDB COMMANDS ==========\n";
    std::cout << "INSERT <id> \"name\" \"value\"\n";
    std::cout << "SELECT <id>\n";
    std::cout << "SELECT ALL\n";
    std::cout << "UPDATE <id> \"name\" \"value\"\n";
    std::cout << "DELETE <id>\n";
    std::cout << "HELP\n";
    std::cout << "EXIT\n";
    std::cout << "======================================\n\n";
}

void printRecord(const Record& record) {
    std::cout << "[ID: " << record.getId()
              << "] Name: " << record.getName()
              << " | Value: " << record.getValue()
              << '\n';
}

int main() {
    Database database;
    Storage storage("data/nexusdb.txt");

    std::cout << "=====================================\n";
    std::cout << "       NEXUSDB DATABASE ENGINE       \n";
    std::cout << "             C++17 v1.0             \n";
    std::cout << "=====================================\n";

    if (!storage.load(database)) {
        std::cerr << "[ERROR] Failed to load database.\n";
        return 1;
    }

    std::cout << "[OK] Database loaded successfully.\n";
    std::cout << "[INFO] Records: " << database.size() << '\n';

    printHelp();

    std::string input;

    while (true) {
        std::cout << "NexusDB> ";

        if (!std::getline(std::cin, input)) {
            break;
        }

        Query query = QueryParser::parse(input);

        switch (query.type) {
            case QueryType::Insert: {
                if (database.insert(
                        query.id,
                        query.name,
                        query.value)) {
                    std::cout << "[OK] Record inserted.\n";
                } else {
                    std::cout << "[ERROR] Invalid or duplicate ID.\n";
                }
                break;
            }

            case QueryType::Select: {
                auto record = database.select(query.id);

                if (record.has_value()) {
                    printRecord(*record);
                } else {
                    std::cout << "[ERROR] Record not found.\n";
                }
                break;
            }

            case QueryType::SelectAll: {
                auto records = database.selectAll();

                if (records.empty()) {
                    std::cout << "[INFO] Database is empty.\n";
                } else {
                    for (const auto& record : records) {
                        printRecord(record);
                    }
                }

                std::cout << "[INFO] Total: "
                          << records.size() << '\n';
                break;
            }

            case QueryType::Update: {
                if (database.update(
                        query.id,
                        query.name,
                        query.value)) {
                    std::cout << "[OK] Record updated.\n";
                } else {
                    std::cout << "[ERROR] Update failed.\n";
                }
                break;
            }

            case QueryType::Delete: {
                if (database.remove(query.id)) {
                    std::cout << "[OK] Record deleted.\n";
                } else {
                    std::cout << "[ERROR] Record not found.\n";
                }
                break;
            }

            case QueryType::Help:
                printHelp();
                break;

            case QueryType::Exit:
                if (!storage.save(database)) {
                    std::cerr << "[ERROR] Failed to save database.\n";
                    return 1;
                }

                std::cout << "[OK] Database saved.\n";
                std::cout << "Goodbye from NexusDB!\n";
                return 0;

            case QueryType::Invalid:
                std::cout << "[ERROR] "
                          << query.error << '\n';
                break;
        }
    }

    if (!storage.save(database)) {
        std::cerr << "[ERROR] Failed to save database.\n";
        return 1;
    }

    std::cout << "\n[OK] Database saved.\n";
    return 0;
}
