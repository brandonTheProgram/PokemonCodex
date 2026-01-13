#ifndef REPOSITORY_H_
#define REPOSITORY_H_

#include "Logger.h"

class SQLManager;

/**
 * @class Repository
 * @brief A base class for querying the repos.
 */
class Repository
{
    protected:
        /**
         * @brief Constructs a new Repository object.
         * @param sqlManager The sql manager instance that allows queries.
         */
        explicit Repository(SQLManager& sqlManager) : logger_(Logger::getInstance()), sqlManager_(sqlManager) {}

        Logger& logger_;
        SQLManager& sqlManager_;

    public:
        /**
         * @brief Destroys the Repository object.
         */
        virtual ~Repository() = default;

        /**
         * @brief Deleted copy constructor.
         */
        Repository(const Repository&) = delete;

        /**
         * @brief Deleted copy assignment operator.
         */
        Repository& operator=(const Repository&) = delete;
};

#endif