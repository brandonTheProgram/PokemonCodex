#include "SQLManager.h"

#include <iostream>

SQLManager::SQLManager(const std::string& dbPath)
    : db_(dbPath, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE), logger_(Logger::getInstance())
{
}

std::vector<std::vector<std::string>> SQLManager::query(
    const std::string& sql, const std::function<void(SQLite::Statement&)>& binder)
{
    this->logger_.debug("SQLManager::query invoked");
    std::lock_guard<std::mutex> lock(this->mutex_);

    try
    {
        SQLite::Statement st(this->db_, sql);

        if (binder) binder(st);

        std::vector<std::vector<std::string>> rows;

        while (st.executeStep())
        {
            std::vector<std::string> row;

            row.reserve(st.getColumnCount());

            for (int i = 0; i < st.getColumnCount(); ++i)
            {
                row.push_back(st.getColumn(i).isNull() ? "NULL" : st.getColumn(i).getText());
            }
            rows.push_back(std::move(row));
        }
        return rows;
    }
    catch (const SQLite::Exception& e)
    {
        this->logger_.critical("SQLManager::query Failed to fetch results. Errors message: " +
                               std::string(e.what()));
        return {};
    }
}