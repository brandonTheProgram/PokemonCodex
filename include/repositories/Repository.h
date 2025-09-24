#ifndef REPOSITORY_H_
#define REPOSITORY_H_

#include "Logger.h"

class SQLManager;

class Respository
{
    protected:
        Respository(SQLManager& sqlManager) : logger_(Logger::getInstance()), sqlManager_(sqlManager) 
        {}

        Logger& logger_;
        SQLManager& sqlManager_;
};

#endif