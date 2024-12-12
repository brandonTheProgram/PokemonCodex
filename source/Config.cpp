#include "Config.h"

#include <json/json.h>

#include <fstream>

Config& Config::getInstance()
{
    static Config instance;
    return instance;
}

void Config::load(const std::string& filePath)
{
    std::lock_guard<std::mutex> lock(configMutex);

    std::ifstream configFile(filePath);
    if (!configFile.is_open())
    {
        throw std::runtime_error("Unable to open config file: " + filePath);
    }

    // Parse the JSON
    Json::Value root;
    Json::CharReaderBuilder builder;
    std::string errs;

    if (!Json::parseFromStream(builder, configFile, &root, &errs))
    {
        throw std::runtime_error("Error parsing JSON: " + errs);
    }

    // Iterate through the JSON and populate the map
    for (const auto& key : root.getMemberNames())
    {
        const auto& value = root[key];
        if (value.isString())
        {
            this->environment[key] = value.asString();
        }
        else
        {
            // Handle other types as needed, converting them to strings
            this->environment[key] = value.toStyledString();
        }
    }
}

std::string Config::get(const std::string& key) const
{
    auto it = this->environment.find(key);
    if (it != this->environment.end())
    {
        return it->second;
    }
    throw std::runtime_error("Key not found in environment: " + key);
}