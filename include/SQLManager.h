#ifndef SQLMANAGER_H_
#define SQLMANAGER_H_

#include <SQLiteCpp/SQLiteCpp.h>

#include <string>
#include <vector>

#include "Logger.h"

class SQLManager
{
    public:
        SQLManager(const std::string& dbPath);
        ~SQLManager() = default;

        void executeQuery(const std::string& query);
        void prepareStatement(const std::string& query);
        void bind(const int& index, const int& value);
        void bind(const int& index, const std::string& value);
        void bind(const int& index, const int* value);
        std::vector<std::vector<std::string>> fetchResults();

    private:
        SQLite::Database db_;
        std::unique_ptr<SQLite::Statement> stmt_;
        Logger& logger;
};

#endif