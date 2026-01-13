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

/**
 * @struct EvolutionData
 * @brief Represents an individual Pokemon's evolutionary data.
 */
struct EvolutionData
{
        std::uint32_t basePokedexNumber;    ///< The Pokemon's Pokedex number.
        std::uint32_t evolvedPokedexNumber; ///< The Evolved Pokemons' Pokedex number.
        std::uint32_t baseRegionId;         ///< The Pokemon's Regional ID.
        std::uint32_t evolvedRegionId;      ///< The Evolved Pokemons' Regional ID.
        std::string evolutionCondition;     ///< The condition how the Pokemon evolves.

        /**
         * @brief Constructs a new EvolutionData object.
         */
        EvolutionData() = default;

        /**
         * @brief Constructs a new EvolutionData object.
         * @param base The Pokemon's Pokedex number.
         * @param evolved The Evolved Pokemons' Pokedex number.
         * @param baseRegion The Pokemon's Regional ID.
         * @param evolvedRegion The Evolved Pokemons' Regional ID.
         * @param condition The condition how the Pokemon evolves.
         */
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

/**
 * @class PokemonRepository
 * @brief An extension of the Repository class for Pokemon.
 * 
 * This class manages the database for anything related to a Pokemon:
 *    Pokemon
 *    Region of Pokemon
 *    Latest Pokemon Added
 *    Pokemon Abilities, Location, & Evolutionary Chain
 */
class PokemonRepository : public Repository
{
    public:
        
        /**
         * @brief Constructs a new PokemonRepository object.
         * @param sqlManager The sql manager instance that allows queries.
         * @param regionRepository The instance of the region repository that allows queries based on a region.
         * @param typeRepository The instance of the type repository that allows queries based on type.
         * @param attackRepository The instance of the attack repository that allows queries based on attacks.
         */
        PokemonRepository(SQLManager& sqlManager, RegionRepository& regionRepository, TypeRepository& typeRepository, AttackRepository& attackRepository);

        /**
         * @brief Destroys the AttackRepository object.
         */
        ~PokemonRepository() = default;

        /**
         * @brief Query for the entire Pokemon's information.
         * @param pokedexNumber The target Pokemon.
         * @param regionalFormId The regional id of the target Pokemon.
         * @param connectorFunction A function pointer used during connecting the Evolutionary Chain.
         * @param buttonFunction A function pointer used during connecting the Evolutionary Chain.
         * @return A Json value object of the results.
         */
        Json::Value queryPokemon(const std::string& pokedexNumber, const std::string& regionalFormId, ConnectorFunction connectorFunction, ButtonFunction buttonFunction);

        /**
         * @brief Query for an entire region of Pokemon.
         * @param region The target region.
         * @param shouldLimit Flag for if there is a limit.
         * @param limit The limit applied to the serach.
         * @return A list of Json value member object of the results.
         */
        std::vector<Json::Value::Members> queryRegionPokemon(const std::string& region, const bool shouldLimit, const uint32_t& limit = 0);
        
        /**
         * @brief Query for a Pokemon via name.
         * @param name The target name.
         * @return A list of Json value member object of the results.
         */
        std::vector<Json::Value::Members> querySearchPokemon(const std::string& name);

        /**
         * @brief Query for the latest (n) number of Pokemon.
         * @param limit The limit applied to the serach.
         * @return A list of Json value member object of the results.
         */
        std::vector<Json::Value::Members> queryLatestsPokemon(const uint32_t limit);

        /**
         * @brief Query for the evolutions of the target Pokemon.
         * @param pokedexNumber The target Pokemon.
         * @param regionalFormId The regional id of the target Pokemon.
         * @return A list of Json value member object of the results.
         */
        std::vector<Json::Value::Members> queryEvolution(const uint32_t& pokedexNumber, const uint32_t& regionalFormId);

        /**
         * @brief Query for the target Pokemon's locations.
         * @param pokedexNumber The target Pokemon.
         * @param regionalFormId The regional id of the target Pokemon.
         * @return A Json value object of the results.
         */
        Json::Value queryLocationTable(const uint32_t pokedexNumber, const std::string& regionalFormId);

        /**
         * @brief Query for the target Pokemon's abilities.
         * @param id The target ability.
         * @return A string result.
         */
        std::string queryAbilityTable(const uint32_t id);

        /**
         * @brief Query for the target Pokemon's evolutionary chain.
         * @param targetPokedexNumber The target Pokemon.
         * @param connectorFunction A function pointer used during connecting the Evolutionary Chain.
         * @param buttonFunction A function pointer used during connecting the Evolutionary Chain.
         * @return A Json value object of the results.
         */
        Json::Value queryEvolutionTable(const uint32_t targetPokedexNumber, ConnectorFunction connectorFunction, ButtonFunction buttonFunction);

    private:
        RegionRepository& regionRepository_;
        TypeRepository& typeRepository_;
        AttackRepository& attackRepository_;
};

#endif