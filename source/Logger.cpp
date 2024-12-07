#include "Logger.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>
#include <cstdlib>

#pragma warning(disable : 4996)

Logger& Logger::getInstance(const std::string& fileName) {
    const char* logDirEnv = std::getenv("LOG_DIR");
    if (logDirEnv != nullptr) {
        std::string logDirStr = logDirEnv;
        static Logger instance(logDirStr, logDirStr + "/" + fileName);
        return instance;
    }

    throw std::invalid_argument("A log directory was not supplied in the CMakeLists. Please add it under: LOG_DIR");
}

Logger::Level Logger::getCurrentLogLevel() {
    const char* logLevelEnv = std::getenv("LOG_LEVEL");
    if (logLevelEnv != nullptr) {
        std::string loglevelStr = logLevelEnv;
        if (loglevelStr == "DEBUG") return Level::DEBUG;
        if (loglevelStr == "INFO") return Level::INFO;
        if (loglevelStr == "WARNING") return Level::WARNING;
        if (loglevelStr == "CRITICAL") return Level::CRITICAL;
    }
    return Level::INFO;  // Default log level is INFO
}

Logger::Logger(const std::string& logDir,
               const std::string& fileName) : logFileName_(fileName), currentLevel_(getCurrentLogLevel()) {
    // Create the directory if it doesn't exist
    if (!std::filesystem::exists(logDir)) {
        std::filesystem::create_directories(logDir);
    }
}

Logger::~Logger() = default;

void Logger::log(const std::string& message, Level level) {
    if (static_cast<int>(level) >= static_cast<int>(currentLevel_)) { 
        std::lock_guard<std::mutex> lock(mutex_);
        std::ofstream logFile(logFileName_, std::ios_base::app);
        if (logFile.is_open()) {
            logFile << "[" << getCurrentTime() << "] [" << levelToString(level) << "] " << message << std::endl;
            logFile.close();
        }
        else {
            std::cerr << "Unable to open/create the log file";
        }
    }
}

void Logger::debug(const std::string& message) {
    log(message, Level::DEBUG);
}

void Logger::info(const std::string& message) {
    log(message, Level::INFO);
}

void Logger::warning(const std::string& message) {
    log(message, Level::WARNING);
}

void Logger::critical(const std::string& message) {
    log(message, Level::CRITICAL);
}

std::string Logger::getCurrentTime() {
    auto now = std::time(nullptr);
    std::tm timeStruct{};
    localtime_s(&timeStruct, &now);
    std::ostringstream oss;
    oss << std::put_time(&timeStruct, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::levelToString(Level level) {
    switch (level) {
        case Level::DEBUG:      return "DEBUG";
        case Level::INFO:       return "INFO";
        case Level::WARNING:    return "WARNING";
        case Level::CRITICAL:   return "CRITICAL";
        default:                return "UNKNOWN";
    }
}
