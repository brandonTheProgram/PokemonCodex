#include "Logger.h"

#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "Config.h"

Logger& Logger::getInstance(const std::string& fileName)
{
    std::string logDir = Config::getInstance().get("LOG_DIR");

    static Logger instance(logDir, logDir + "/" + fileName);
    return instance;
}

Logger::Level Logger::getCurrentLogLevel()
{
    std::string logLevel = Config::getInstance().get("LOG_LEVEL");

    if (logLevel == "DEBUG") return Level::DEBUG;
    if (logLevel == "INFO") return Level::INFO;
    if (logLevel == "WARNING") return Level::WARNING;
    if (logLevel == "CRITICAL") return Level::CRITICAL;

    return Level::INFO;  // Default log level is INFO
}

Logger::Logger(const std::string& logDir, const std::string& fileName)
    : logFileName_(fileName), currentLevel_(this->getCurrentLogLevel())
{
    // Create the directory if it doesn't exist
    if (!std::filesystem::exists(logDir))
    {
        std::filesystem::create_directories(logDir);
    }

    this->toggleLogFile();

    if (!this->logFile_.is_open())
    {
        std::cerr << "Unable to open/create the log file: " << fileName
                  << " in the directory: " << logDir;
    }
}

Logger::~Logger()
{
    this->toggleLogFile();
}

void Logger::log(const std::string& message, Level level)
{
    if (static_cast<int>(level) >= static_cast<int>(this->currentLevel_))
    {
        std::lock_guard<std::mutex> lock(this->loggerMutex_);

        if (this->logFile_.is_open())
        {
            this->logFile_ << "[" << this->getCurrentTime() << "] [" << this->levelToString(level)
                           << "] " << message << std::endl;
        }
    }
}

void Logger::debug(const std::string& message)
{
    this->log(message, Level::DEBUG);
}

void Logger::info(const std::string& message)
{
    this->log(message, Level::INFO);
}

void Logger::warning(const std::string& message)
{
    this->log(message, Level::WARNING);
}

void Logger::critical(const std::string& message)
{
    this->log(message, Level::CRITICAL);
}

std::string Logger::getCurrentLogs()
{
    if (this->logFile_.is_open())
    {
        this->toggleLogFile();
    }
    std::ifstream logFile(this->logFileName_);

    if (!logFile.is_open())
    {
        this->warning("Failed to open the log file to get the logs for the front end");
        return "";
    }

    std::stringstream buffer;
    buffer << logFile.rdbuf();
    logFile.close();

    if (!this->logFile_.is_open())
    {
        this->toggleLogFile();
    }

    return buffer.str();
}

std::string Logger::getCurrentTime() const
{
    auto now = std::time(nullptr);
    std::tm timeStruct{};
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&timeStruct, &now);
#else
    localtime_r(&now, &timeStruct);
#endif
    std::ostringstream oss;
    oss << std::put_time(&timeStruct, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::levelToString(const Level& level) const
{
    switch (level)
    {
        case Level::DEBUG:
            return "DEBUG";
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

void Logger::toggleLogFile()
{
    if (this->logFile_.is_open())
    {
        this->logFile_.close();
    }
    else
    {
        this->logFile_.open(this->logFileName_, std::ios_base::app);
    }
}
