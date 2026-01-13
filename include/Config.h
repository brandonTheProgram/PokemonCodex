#ifndef CONFIG_H_
#define CONFIG_H_

#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>

/**
 * @class Config
 * @brief Manages environment variables.
 */
class Config
{
    public:
        /**
         * @brief Return an instance of the Config object.
         * @return An instance of the Config object.
         */
        static Config& getInstance();

        /**
         * @brief Load the environment variables from a file.
         * @param filePath The path to the environment variables.
         */
        void load(const std::string& filePath);

        /**
         * @brief Try adding an environment variable.
         * @param key The environment variable key.
         * @param value The value of the environment variable.
         * @return The value of the environment variable.
         */
        bool addEnvVar(const std::string& key, const std::string& value);

        /**
         * @brief Get the value of the environment variable.
         * @param key The environment variable key.
         * @return The value of the environment variable.
         */
        std::string get(const std::string& key) const;

    private:
        /**
         * @brief Constructs a new Config object.
         */
        Config() = default;

        std::unordered_map<std::string, std::string> environment_;
        std::mutex configMutex_;
};

#endif