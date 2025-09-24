#ifndef POKEMONREPOSITORY_H_
#define POKEMONREPOSITORY_H_

#include <optional>
#include <vector>
#include <functional>

#include <json/json.h>

#include "Repository.h"

class SQLManager;
class RegionRepository;
class TypeRepository;
class AttackRepository;

struct EvolutionData
{
        std::uint32_t basePokedexNumber;
        std::uint32_t evolvedPokedexNumber;
        std::uint32_t baseRegionId;
        std::uint32_t evolvedRegionId;
        std::string evolutionCondition;

        EvolutionData() = default;

        EvolutionData(const uint32_t base, const uint32_t evolved,
                    const uint32_t baseRegion, const uint32_t evolvedRegion,
                    const std::string& condition)
            : basePokedexNumber(base),
            evolvedPokedexNumber(evolved),
            baseRegionId(baseRegion),
            evolvedRegionId(evolvedRegion),
            evolutionCondition(condition)
        {
        }
};

using ConnectorFunction = std::function<void(std::vector<EvolutionData>&)>;
using ButtonFunction = std::function<Json::Value(const std::vector<std::vector<std::string>>&)>;

class PokemonRepository : public Repository
{
    public:
        

        PokemonRepository(SQLManager& sqlManager, RegionRepository& regionRepository, TypeRepository& typeRepository, AttackRepository& attackRepository);

        ~PokemonRepository() = default;

        Json::Value queryPokemon(const std::string& pokedexNumber, const std::string& regionalFormId, ConnectorFunction connectorFunction, ButtonFunction buttonFunction);

        std::vector<Json::Value::Members> queryRegionPokemon(const std::string& region, const bool shouldLimit, const uint32_t& limit = 0);
        
        std::vector<Json::Value::Members> querySearchPokemon(const std::string& name);

        std::vector<Json::Value::Members> queryLatestsPokemon(const uint32_t limit);

        std::vector<Json::Value::Members> queryEvolution(const uint32_t& pokedexNumber, const uint32_t& regionalFormId);

        Json::Value queryLocationTable(const uint32_t pokedexNumber, const std::string& regionalFormId);

        std::string queryAbilityTable(const uint32_t id);

        Json::Value queryEvolutionTable(const uint32_t targetPokedexNumber, ConnectorFunction connectorFunction, ButtonFunction buttonFunction);

    private:
        RegionRepository& regionRepository_;
        TypeRepository& typeRepository_;
        AttackRepository& attackRepository_;
};

#endif