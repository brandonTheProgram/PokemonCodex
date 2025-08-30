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
    std::lock_guard<std::mutex> lock(this->configMutex_);

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
            this->environment_[key] = value.asString();
        }
        else
        {
            // Handle other types as needed, converting them to strings
            this->environment_[key] = value.toStyledString();
        }
    }
}

bool Config::addEnvVar(const std::string& key, const std::string& value)
{
    auto result = this->environment_.insert({key, value});

    if (!result.second)
    {
        return false;
    }

    return true;
}

std::string Config::get(const std::string& key) const
{
    auto it = this->environment_.find(key);
    if (it != this->environment_.end())
    {
        return it->second;
    }
    throw std::runtime_error("Key not found in environment: " + key);
}