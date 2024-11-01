#include "SQLManager.h"
#include <iostream>

SQLManager::SQLManager(const std::string& dbPath)
    : db_(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), stmt_(nullptr) {}

void SQLManager::executeQuery(const std::string& query) {
    try {
        db_.exec(query);
    } catch (const SQLite::Exception& e) {
        std::cerr << "Failed to execute query: " << query << " - " << std::string(e.what()) << std::endl;
    }
}

void SQLManager::prepareStatement(const std::string& query) {
    try {
        stmt_ = std::make_unique<SQLite::Statement>(db_, query);
    } catch (const SQLite::Exception& e) {
        std::cerr << "Failed to execute query: " << query << " - " << std::string(e.what()) << std::endl;
    }
}

std::vector<std::vector<std::string>> SQLManager::fetchResults() {
    std::vector<std::vector<std::string>> results;

    try {
        while (stmt_->executeStep()) {
            std::vector<std::string> row;
            for (int i = 0; i < stmt_->getColumnCount(); i++) {
                row.push_back(stmt_->getColumn(i).isNull() ? "NULL" : stmt_->getColumn(i).getText());
            }
            results.push_back(row);
        }
    } catch (const SQLite::Exception& e) {
        std::cerr << "Failed to fetch results: " << std::string(e.what()) << std::endl;
    }
    
    stmt_->reset();
    return results;
}