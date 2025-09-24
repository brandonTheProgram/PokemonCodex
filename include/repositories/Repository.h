#ifndef REPOSITORY_H_
#define REPOSITORY_H_

#include "Logger.h"

class SQLManager;

class Repository
{
    protected:
        explicit Repository(SQLManager& sqlManager) : logger_(Logger::getInstance()), sqlManager_(sqlManager) {}

        Logger& logger_;
        SQLManager& sqlManager_;

    public:
        virtual ~Repository() = default;
        Repository(const Repository&) = delete;
        Repository& operator=(const Repository&) = delete;
};

#endif