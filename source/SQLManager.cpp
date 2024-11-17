#include "SQLManager.h"
#include <iostream>

SQLManager::SQLManager(const std::string& dbPath)
    : db_(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), stmt_(nullptr), logger(Logger::getInstance()) {}

void SQLManager::executeQuery(const std::string& query) {
    try {
        db_.exec(query);
    } catch (const SQLite::Exception& e) {
        logger.critical("SQLManager::executeQuery Failed to execute query: " + query + ". Error message: " + std::string(e.what()));
    }
}

void SQLManager::prepareStatement(const std::string& query) {
    try {
        stmt_ = std::make_unique<SQLite::Statement>(db_, query);
    } catch (const SQLite::Exception& e) {
        logger.critical("SQLManager::prepareStatement Failed to prepare the statement for the query: " + query + ". Error message: " + std::string(e.what()));
    }
}

void SQLManager::bind(const int& index, const int& value) {
    try {
        stmt_->bind(index, value);
    } catch (const SQLite::Exception& e) {
        logger.critical("SQLManager::bind Failed to bind: " + std::to_string(value) + " to index: " + std::to_string(index) + ". Error message: " + std::string(e.what()));
    }
}

void SQLManager::bind(const int& index, const std::string& value) {
    try {
        stmt_->bind(index, value);
    } catch (const SQLite::Exception& e) {
        logger.critical("SQLManager::bind Failed to bind: " + value + " to index: " + std::to_string(index) + ". Error message: " + std::string(e.what()));
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
        logger.critical("SQLManager::fetchResults Failed to fetch results. Errors message: " + std::string(e.what()));
        std::cerr << "Failed to fetch results: " << std::string(e.what()) << std::endl;
    }
    
    stmt_->reset();
    return results;
}