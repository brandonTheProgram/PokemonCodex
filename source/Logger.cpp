#include "Logger.h"
#include "Config.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

Logger& Logger::getInstance(const std::string& fileName) {
    std::string logDir = Config::getInstance().get("LOG_DIR");
    
    static Logger instance(logDir, logDir + "/" + fileName);
    return instance;
}

Logger::Level Logger::getCurrentLogLevel() {
    std::string logLevel = Config::getInstance().get("LOG_LEVEL");

    if (logLevel == "DEBUG") return Level::DEBUG;
    if (logLevel == "INFO") return Level::INFO;
    if (logLevel == "WARNING") return Level::WARNING;
    if (logLevel == "CRITICAL") return Level::CRITICAL;

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
