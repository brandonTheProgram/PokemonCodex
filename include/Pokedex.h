#ifndef POKEDEX_H_
#define POKEDEX_H_

#include <json/json.h>

#include <functional>
#include <memory>
#include <utility>

#include "SQLManager.h"

class Logger;
class AttackRepository;
class GameRepository;
class PokemonRepository;
class RegionRepository;
class TypeRepository;

struct EvolutionData;

#ifndef DOXYGEN_SHOULD_SKIP_THIS
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
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/**
 * @class Pokedex
 * @brief Manage the Pokedex for the application.
 *
 * This class manages the Pokedex for the application and acts as the CPU.
 */
class Pokedex
{
    public:
        /**
         * @brief Constructs a new Pokedex object.
         */
        Pokedex();

        /**
         * @brief Destroys the Pokedex object.
         */
        ~Pokedex() = default;

        /**
         * @brief Get a region of Pokemon data.
         * @param region The target region.
         * @param shouldLimit A flag if there should be a limit.
         * @return A Json value object of the results.
         */
        Json::Value getRegionPokemonData(const std::string& region, const bool shouldLimit = false);

        /**
         * @brief Query for the latest Pokemon.
         * @return A Json value object of the results.
         */
        Json::Value getLatestsPokemon();

        /**
         * @brief Query for the entire Pokemon's information.
         * @param pokedexNumber The target Pokemon.
         * @param regionalFormId The regional id of the target Pokemon.
         * @return A Json value object of the results.
         */
        Json::Value getPokemonData(const std::string& pokedexNumber,
                                   const std::string& regionalFormId);

        /**
         * @brief Query for a Pokemon via name.
         * @param name The target name.
         * @return A Json value object of the results.
         */
        Json::Value getSearchPokemon(const std::string& name);

        /**
         * @brief Query for a Pokemon types.
         * @return A Json value object of the results.
         */
        Json::Value getPokemonTypes() const;

        /**
         * @brief Query for Pokemon regions.
         * @return A Json value object of the results.
         */
        Json::Value getRegionData() const;

        /**
         * @brief Query for Pokemon Games.
         * @return A Json value object of the results.
         */
        Json::Value getPokemonGames() const;

        /**
         * @brief Query for Pokemon Mainline Games.
         * @return A Json value object of the results.
         */
        Json::Value getPokemonMainlineGames() const;

    private:
        /**
         * @brief Connect the Pokemon's Evolutionary Chain.
         * @param evolutionaryChain A vector holding the evolutionary chain.
         */
        void connectEvolutionaryChain(std::vector<EvolutionData>& evolutionaryChain) const;

        /**
         * @brief Get the limit environment variable.
         * @param latest A flag if the latest limit should be obtained.
         * @return The value of the limit environment variable
         */
        uint32_t getLimitEnvVar(const bool latest = false) const;

        /**
         * @brief Turn results from the database into a Button
         * @param results The results from a database query.
         * @return A Json value object of the results.
         */
        Json::Value pokemonButtonDataToJsonValue(
            const std::vector<std::vector<std::string>>& results);

        std::shared_ptr<SQLManager> sqlManager_;
        Logger& logger_;
        std::shared_ptr<GameRepository> gameRepository_;
        std::shared_ptr<TypeRepository> typeRepository_;
        std::shared_ptr<AttackRepository> attackRepository_;
        std::shared_ptr<RegionRepository> regionRepository_;
        std::shared_ptr<PokemonRepository> pokemonRepository_;
};

#endif