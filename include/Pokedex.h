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
        void connectEvolutionaryChain(std::vector<EvolutionData>& evolutionaryChain) const;

        std::uint32_t getLimitEnvVar(const bool& latest = false) const;

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