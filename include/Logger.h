#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <mutex>
#include <fstream>

class Logger {
    public:
        enum class Level {
            INFO,
            WARNING,
            CRITICAL
        };

        // Get the singleton instance
        static Logger& getInstance();

        // Log a message with a specific level
        void log(const std::string& message, Level level);

        // Utility methods for specific levels
        void info(const std::string& message);
        void warning(const std::string& message);
        void critical(const std::string& message);

    private:
        Logger();                      // Private constructor
        ~Logger();                     // Private destructor
        Logger(const Logger&) = delete;            // Delete copy constructor
        Logger& operator=(const Logger&) = delete; // Delete assignment operator

        std::string getCurrentTime();                 // Helper to get current time
        std::string levelToString(Level level);       // Helper to convert enum to string

        std::string logFileName_;                     // Name of the log file
        std::mutex mutex_;                            // To ensure thread safety
};

#endif // LOGGER_H