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

struct EvolutionData
{
        std::uint32_t basePokedexNumber;
        std::uint32_t evolvedPokedexNumber;
        std::uint32_t baseRegionId;
        std::uint32_t evolvedRegionId;
        std::string evolutionCondition;

        EvolutionData() = default;

        EvolutionData(const std::uint32_t& base, const std::uint32_t& evolved,
                      const std::uint32_t& baseRegion, const std::uint32_t& evolvedRegion,
                      const std::string& condition)
            : basePokedexNumber(base),
              evolvedPokedexNumber(evolved),
              baseRegionId(baseRegion),
              evolvedRegionId(evolvedRegion),
              evolutionCondition(condition)
        {
        }
};

struct pair_hash
{
        template <class T1, class T2>
        std::size_t operator()(const std::pair<T1, T2>& pair) const
        {
            auto h1 = std::hash<T1>{}(pair.first);
            auto h2 = std::hash<T2>{}(pair.second);
            return h1 ^ (h2 << 1);  // Combine hashes
        }
};

class Pokedex
{
    public:
        Pokedex();
        ~Pokedex() = default;

        Json::Value getRegionPokemonData(const std::string& region, const bool shouldLimit = false);
        Json::Value getLatestsPokemon();
        Json::Value getPokemonData(const std::string& pokedexNumber,
                                   const std::string& regionalFormId);
        Json::Value getSearchPokemon(const std::string& name);
        Json::Value getPokemonTypes() const;
        Json::Value getRegionData() const;
        Json::Value getPokemonGames() const;
        Json::Value getPokemonMainlineGames() const;

    private:
        void initializeRegions();
        void initializePokemonTypes();
        void initializeGames();
        void initializeMainlineGames();
        void connectEvolutionaryChain(std::vector<EvolutionData>& evolutionaryChain) const;

        std::uint32_t getLimitEnvVar(const bool& latest = false) const;

        std::string queryRegionalFormTable(const std::uint32_t& id);
        std::string queryTypeTable(const std::uint32_t& id);
        std::string queryAbilityTable(const std::uint32_t& id);
        std::string queryMoveCategoryTable(const std::uint32_t& id);

        Json::Value queryLevelUpMovesetTable(const std::uint32_t& pokedexNumber,
                                             const std::string& regionalFormId);
        std::pair<Json::Value, Json::Value> queryTechnicalMovesetTable(
            const std::uint32_t& pokedexNumber, const std::string& regionalFormId);
        Json::Value queryMoveTable(const std::uint32_t& id);
        Json::Value queryTechnicalMoveTable(const std::uint32_t& id);
        Json::Value queryTypeEffectivnessTable(const std::uint32_t& primaryTypeId,
                                               const std::uint32_t& secondaryTypeId);
        Json::Value queryEvolutionTable(const std::uint32_t& targetPokedexNumber);
        Json::Value queryLocationTable(const std::uint32_t& pokedexNumber,
                                       const std::string& regionalFormId);
        Json::Value pokemonButtonDataToJsonValue(
            const std::vector<std::vector<std::string>>& results);

        SQLManager sqlManager_;
        Logger& logger_;
        std::vector<std::string> keys_;
        std::vector<std::string> pokemonTypes_;
        std::vector<std::string> pokemonGames_;
        std::unordered_map<std::string, std::string> pokemonMainlineGames_;
        std::unordered_map<std::string, Region> regions_;
};

#endif