#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <mutex>
#include <string>

/**
 * @class Logger
 * @brief Manage the logger for the application.
 *
 * This class manages the logger for the application.
 */
class Logger
{
    public:
        /**
         * @enum Level
         * @brief The different log levels
         */
        enum class Level
        {
            DEBUG,    ///< The Debug log level.
            INFO,     ///< The Info log level.
            WARNING,  ///< The Warning log level.
            CRITICAL  ///< The Critical log level.
        };

        /**
         * @brief Return an instance of the Logger object.
         * @return An instance of the Logger object.
         */
        static Logger& getInstance(const std::string& fileName = "log.txt");

        /**
         * @brief Return the Log Level of the Logger object.
         * Get the current log level from environment variable or default to INFO
         * @return An log level of the Logger object.
         */
        static Level getCurrentLogLevel();

        /**
         * @brief Log a message based on the log level.
         * @param message The message to log.
         * @param level The log level.
         */
        void log(const std::string& message, Level level);

        /**
         * @brief Log a debug message.
         * @param message The message to log.
         */
        void debug(const std::string& message);

        /**
         * @brief Log an info message.
         * @param message The message to log.
         */
        void info(const std::string& message);

        /**
         * @brief Log a warning message.
         * @param message The message to log.
         */
        void warning(const std::string& message);

        /**
         * @brief Log a critical message.
         * @param message The message to log.
         */
        void critical(const std::string& message);

        // Retrieve the current logs
        /**
         * @brief Return the current logs.
         * @return the current logs.
         */
        std::string getCurrentLogs();

    private:
        /**
         * @brief Constructs a new Logger object.
         * @param logDir the directory to print the logs
         * @param fileName the name of the output file log
         */
        Logger(const std::string& logDir, const std::string& fileName);

        /**
         * @brief Destroys the Logger object.
         */
        ~Logger();

        /**
         * @brief Delete copy constructor.
         */
        Logger(const Logger&) = delete;  // Delete copy constructor

        /**
         * @brief Delete assignment operator.
         */
        Logger& operator=(const Logger&) = delete;

        /**
         * @brief Get the current time.
         * @return a string representation of the current time
         */
        std::string getCurrentTime() const;

        /**
         * @brief Convert the enum to string.
         * @param level The log level
         * @return a string representation of the current log level
         */
        std::string levelToString(const Level& level) const;

        /**
         * @brief Toggle the log file on and off.
         */
        void toggleLogFile();

        std::ofstream logFile_;
        std::string logFileName_;
        std::mutex loggerMutex_;
        Level currentLevel_;
};

#endif  // LOGGER_H