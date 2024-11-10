#ifndef SQLMANAGER_H_
#define SQLMANAGER_H_

#include <SQLiteCpp/SQLiteCpp.h>
#include <string>
#include <vector>

class SQLManager
{
    public:
        SQLManager(const std::string& dbPath);
        ~SQLManager() = default;

        void executeQuery(const std::string& query);
        void prepareStatement(const std::string& query);
        std::vector<std::vector<std::string>> fetchResults();

    private:
        SQLite::Database db_;
        std::unique_ptr<SQLite::Statement> stmt_;
};

#endif