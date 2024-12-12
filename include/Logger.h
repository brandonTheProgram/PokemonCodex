#ifndef LOGGER_H
#define LOGGER_H

#include <mutex>
#include <string>

class Logger
{
    public:
        enum class Level
        {
            DEBUG,
            INFO,
            WARNING,
            CRITICAL
        };

        // Get the singleton instance
        static Logger& getInstance(const std::string& fileName = "log.txt");

        // Get the current log level from environment variable or default to INFO
        static Level getCurrentLogLevel();

        // Log a message with a specific level
        void log(const std::string& message, Level level);

        // Utility methods for specific levels
        void debug(const std::string& message);
        void info(const std::string& message);
        void warning(const std::string& message);
        void critical(const std::string& message);

    private:
        Logger(const std::string& logDir,
               const std::string& fileName);                  // Private constructor
        ~Logger();                                            // Private destructor
        Logger(const Logger&) = delete;                       // Delete copy constructor
        Logger& operator=(const Logger&) = delete;            // Delete assignment operator

        std::string getCurrentTime() const;                   // Helper to get current time
        std::string levelToString(const Level& level) const;  // Helper to convert enum to string

        std::string logFileName;                              // Name of the log file
        std::mutex loggerMutex;                               // To ensure thread safety
        Level currentLevel;                                   // Current log level
};

#endif                                                        // LOGGER_H