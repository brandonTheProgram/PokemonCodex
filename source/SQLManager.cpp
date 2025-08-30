#include "SQLManager.h"

#include <iostream>

SQLManager::SQLManager(const std::string& dbPath)
    : db_(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE),
      stmt_(nullptr),
      logger_(Logger::getInstance())
{
}

void SQLManager::executeQuery(const std::string& query)
{
    this->logger_.debug("SQLManager::executeQuery invoked");

    try
    {
        this->db_.exec(query);
    }
    catch (const SQLite::Exception& e)
    {
        this->logger_.critical("SQLManager::executeQuery Failed to execute query: " + query +
                               ". Error message: " + std::string(e.what()));
    }
}

void SQLManager::prepareStatement(const std::string& query)
{
    this->logger_.debug("SQLManager::prepareStatement invoked");

    try
    {
        this->stmt_ = std::make_unique<SQLite::Statement>(this->db_, query);
    }
    catch (const SQLite::Exception& e)
    {
        this->logger_.critical(
            "SQLManager::prepareStatement Failed to prepare the statement for the query: " + query +
            ". Error message: " + std::string(e.what()));
    }
}

void SQLManager::bind(const int& index, const int& value)
{
    this->logger_.debug("SQLManager::bind invoked to bind " + std::to_string(value) + " to index " +
                        std::to_string(index));

    try
    {
        this->stmt_->bind(index, value);
    }
    catch (const SQLite::Exception& e)
    {
        this->logger_.critical("SQLManager::bind Failed to bind: " + std::to_string(value) +
                               " to index: " + std::to_string(index) +
                               ". Error message: " + std::string(e.what()));
    }
}

void SQLManager::bind(const int& index, const std::string& value)
{
    this->logger_.debug("SQLManager::bind invoked to bind " + value + " to index " +
                        std::to_string(index));

    try
    {
        this->stmt_->bind(index, value);
    }
    catch (const SQLite::Exception& e)
    {
        this->logger_.critical("SQLManager::bind Failed to bind: " + value + " to index: " +
                               std::to_string(index) + ". Error message: " + std::string(e.what()));
    }
}

void SQLManager::bind(const int& index, const int* value)
{
    (void)value;
    this->logger_.debug("SQLManager::bind invoked to bind NULL to index " + std::to_string(index));

    try
    {
        this->stmt_->bind(index);
    }
    catch (const SQLite::Exception& e)
    {
        this->logger_.critical("SQLManager::bind Failed to bind: NULL to index: " +
                               std::to_string(index) + ". Error message: " + std::string(e.what()));
    }
}

std::vector<std::vector<std::string>> SQLManager::fetchResults()
{
    this->logger_.debug("SQLManager::fetchResults");

    std::vector<std::vector<std::string>> results;

    try
    {
        while (this->stmt_->executeStep())
        {
            std::vector<std::string> row;
            for (int i = 0; i < this->stmt_->getColumnCount(); i++)
            {
                row.push_back(this->stmt_->getColumn(i).isNull()
                                  ? "NULL"
                                  : this->stmt_->getColumn(i).getText());
            }
            results.push_back(row);
        }
    }
    catch (const SQLite::Exception& e)
    {
        this->logger_.critical(
            "SQLManager::fetchResults Failed to fetch results. Errors message: " +
            std::string(e.what()));
    }

    this->stmt_->reset();
    return results;
}