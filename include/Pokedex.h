#ifndef POKEDEX_H_
#define POKEDEX_H_

#include <json/json.h>

#include <unordered_map>
#include <utility>

#include "SQLManager.h"

struct Region
{
        std::string name;
        std::string image;
        std::uint32_t start;
        std::uint32_t end;

        Region() = default;
        Region(const std::string& name, const std::string& image, const std::uint32_t& start,
               const std::uint32_t& end)
            : name(name), image(image), start(start), end(end)
        {
        }
};

class Pokedex
{
    public:
        Pokedex();
        ~Pokedex() = default;

        Json::Value getRegionPokemonData(const std::string& region,
                                         const bool& shouldLimit = false);
        Json::Value getLatestsPokemon();
        Json::Value getRegionData() const;

    private:
        void initializeRegions();
        std::string getRegionalFormName(const std::uint32_t& id);
        Json::Value pokemonButtonDataToJsonValue(
            const std::vector<std::vector<std::string>>& results);
        std::uint32_t getLimitEnvVar(const bool& latest = false) const;

        SQLManager sqlManager;
        Logger& logger;
        std::vector<std::string> keys;
        std::unordered_map<std::string, Region> regions;
};

#endif