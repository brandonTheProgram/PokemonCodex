#ifndef SQLMANAGER_H_
#define SQLMANAGER_H_

#include <SQLiteCpp/SQLiteCpp.h>

#include <functional>
#include <mutex>
#include <string>
#include <vector>

#include "Logger.h"

/**
 * @class SQLManager
 * @brief Manage the SQL connection for the application.
 *
 * This class manages the SQL connection for the application.
 */
class SQLManager
{
    public:
        /**
         * @brief Constructs a new SQLManager object.
         * @param dbPath The path to the database
         */
        SQLManager(const std::string& dbPath);

        /**
         * @brief Destroys the SQLManager object.
         */
        ~SQLManager() = default;

        /**
         * @brief query given an sql string.
         * @param sql The sql string to execute.
         * @param binder A function pointer to bind the SQL statement.
         */
        std::vector<std::vector<std::string>> query(
            const std::string& sql,
            const std::function<void(SQLite::Statement&)>& binder = nullptr);

    private:
        SQLite::Database db_;
        Logger& logger_;
        mutable std::mutex mutex_;
};

#endif