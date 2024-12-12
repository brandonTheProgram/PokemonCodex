#ifndef CONFIG_H_
#define CONFIG_H_

#include <mutex>
#include <stdexcept>
#include <string>
#include <unordered_map>

class Config
{
    public:
        // Get the singleton instance
        static Config& getInstance();

        void load(const std::string& filePath);

        std::string get(const std::string& key) const;

    private:
        Config() = default;
        std::unordered_map<std::string, std::string> environment;
        std::mutex configMutex;
};

#endif