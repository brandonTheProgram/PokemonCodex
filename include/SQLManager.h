#ifndef SQLMANAGER_H_
#define SQLMANAGER_H_

#include <SQLiteCpp/SQLiteCpp.h>

#include <functional>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

#include "Logger.h"

class SQLManager
{
    public:
        SQLManager(const std::string& dbPath);
        ~SQLManager() = default;

        std::vector<std::vector<std::string>> query(
            const std::string& sql,
            const std::function<void(SQLite::Statement&)>& binder = nullptr);

    private:
        SQLite::Database db_;
        Logger& logger_;
        mutable std::mutex mutex_;
};

#endif