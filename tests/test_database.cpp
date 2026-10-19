
#include "Database.hpp"
#include "QueryParser.hpp"
#include "Storage.hpp"

#include <cassert>
#include <filesystem>
#include <iostream>
#include <string>

void testInsert() {
    Database db;

    assert(db.insert(1, "Andrew", "Developer"));
    assert(db.size() == 1);
    assert(db.contains(1));

    std::cout << "[PASS] INSERT\n";
}

void testDuplicateId() {
    Database db;

    assert(db.insert(1, "Andrew", "Developer"));
    assert(!db.insert(1, "Other", "Engineer"));
    assert(db.size() == 1);

    std::cout << "[PASS] DUPLICATE ID\n";
}

void testSelect() {
    Database db;

    db.insert(1, "Andrew", "C++ Developer");

    auto record = db.select(1);

    assert(record.has_value());
    assert(record->getName() == "Andrew");
    assert(record->getValue() == "C++ Developer");

    assert(!db.select(999).has_value());

    std::cout << "[PASS] SELECT\n";
}

void testUpdate() {
    Database db;

    db.insert(1, "Andrew", "Junior");

    assert(db.update(1, "Andrew", "Mid Developer"));

    auto record = db.select(1);

    assert(record.has_value());
    assert(record->getValue() == "Mid Developer");

    assert(!db.update(999, "Missing", "Value"));

    std::cout << "[PASS] UPDATE\n";
}

void testDelete() {
    Database db;

    db.insert(1, "Andrew", "Developer");

    assert(db.remove(1));
    assert(!db.contains(1));
    assert(db.empty());

    assert(!db.remove(1));

    std::cout << "[PASS] DELETE\n";
}

void testSelectAll() {
    Database db;

    db.insert(3, "Third", "C");
    db.insert(1, "First", "A");
    db.insert(2, "Second", "B");

    auto records = db.selectAll();

    assert(records.size() == 3);
    assert(records[0].getId() == 1);
    assert(records[1].getId() == 2);
    assert(records[2].getId() == 3);

    std::cout << "[PASS] SELECT ALL\n";
}

void testInvalidRecords() {
    Database db;

    assert(!db.insert(0, "Invalid", "Value"));
    assert(!db.insert(-5, "Invalid", "Value"));
    assert(!db.insert(1, "", "Value"));

    assert(db.empty());

    std::cout << "[PASS] VALIDATION\n";
}

void testQueryParser() {
    Query insert = QueryParser::parse(
        "INSERT 1 \"Andrew\" \"Developer\""
    );

    assert(insert.type == QueryType::Insert);
    assert(insert.id == 1);
    assert(insert.name == "Andrew");
    assert(insert.value == "Developer");

    Query select = QueryParser::parse("SELECT 1");
    assert(select.type == QueryType::Select);
    assert(select.id == 1);

    Query all = QueryParser::parse("SELECT ALL");
    assert(all.type == QueryType::SelectAll);

    Query update = QueryParser::parse(
        "UPDATE 1 \"Andrew\" \"Engineer\""
    );
    assert(update.type == QueryType::Update);

    Query remove = QueryParser::parse("DELETE 1");
    assert(remove.type == QueryType::Delete);

    Query invalid = QueryParser::parse("UNKNOWN");
    assert(invalid.type == QueryType::Invalid);

    std::cout << "[PASS] QUERY PARSER\n";
}

void testStorage() {
    namespace fs = std::filesystem;

    const fs::path testPath =
        fs::temp_directory_path() / "nexusdb_test.txt";

    Storage storage(testPath.string());

    Database original;

    original.insert(1, "Andrew", "Developer");
    original.insert(2, "NexusDB", "Database Engine");

    assert(storage.save(original));

    Database loaded;
    assert(storage.load(loaded));

    assert(loaded.size() == 2);

    auto record = loaded.select(1);

    assert(record.has_value());
    assert(record->getName() == "Andrew");
    assert(record->getValue() == "Developer");

    fs::remove(testPath);

    std::cout << "[PASS] STORAGE\n";
}

void testClear() {
    Database db;

    db.insert(1, "Andrew", "Developer");
    db.insert(2, "NexusNet", "Networking");

    db.clear();

    assert(db.empty());
    assert(db.size() == 0);

    std::cout << "[PASS] CLEAR\n";
}

int main() {
    std::cout << "\n===== NEXUSDB AUTOMATED TESTS =====\n\n";

    testInsert();
    testDuplicateId();
    testSelect();
    testUpdate();
    testDelete();
    testSelectAll();
    testInvalidRecords();
    testQueryParser();
    testStorage();
    testClear();

    std::cout << "\n[OK] ALL 10 TESTS PASSED!\n";

    return 0;
}
