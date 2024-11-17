#include "Logger.h"
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <filesystem>

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : logFileName_("../logs/log.txt") {
    // Create the directory if it doesn't exist
    if (!std::filesystem::exists("../logs")) {
        std::filesystem::create_directories("../logs");
    }
}

Logger::~Logger() = default;

void Logger::log(const std::string& message, Level level) {
    std::lock_guard<std::mutex> lock(mutex_);
    std::ofstream logFile(logFileName_, std::ios_base::app);
    if (logFile.is_open()) {
        logFile << "[" << getCurrentTime() << "] [" << levelToString(level) << "] " << message << std::endl;
        logFile.close();
    }
    else {
        std::cerr << "Unable to open/create the file"; 
    }
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
        case Level::INFO:
            return "INFO";
        case Level::WARNING:
            return "WARNING";
        case Level::CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
    }
}
