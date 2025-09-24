
#include "Pokedex.h"

#include "Config.h"
#include "Logger.h"
#include "repositories/AttackRepository.h"
#include "repositories/GameRepository.h"
#include "repositories/PokemonRepository.h"
#include "repositories/RegionRepository.h"
#include "repositories/TypeRepository.h"

Pokedex::Pokedex()
    : sqlManager_(std::make_shared<SQLManager>(Config::getInstance().get("DATABASE_PATH"))),
      logger_(Logger::getInstance()),
      gameRepository_(std::make_shared<GameRepository>(*sqlManager_)),
      typeRepository_(std::make_shared<TypeRepository>(*sqlManager_)),
      attackRepository_(std::make_shared<AttackRepository>(*sqlManager_, *typeRepository_)),
      regionRepository_(std::make_shared<RegionRepository>(*sqlManager_)),
      pokemonRepository_(std::make_shared<PokemonRepository>(*sqlManager_, *regionRepository_,
                                                             *typeRepository_, *attackRepository_))
{
}

Json::Value Pokedex::getRegionPokemonData(const std::string& region, const bool shouldLimit)
{
    this->logger_.debug("Pokedex::getRegionPokemonData invoked for " + region);

    std::vector<Json::Value::Members> results;

    if (shouldLimit)
    {
        results = this->pokemonRepository_->queryRegionPokemon(region, shouldLimit,
                                                               this->getLimitEnvVar());
    }
    else
    {
        results = this->pokemonRepository_->queryRegionPokemon(region, shouldLimit);
    }

    if (results.empty())
    {
        return Json::Value{};
    }
    else
    {
        return this->pokemonButtonDataToJsonValue(results);
    }
}

Json::Value Pokedex::getSearchPokemon(const std::string& name)
{
    this->logger_.debug("Pokedex::getSearchPokemon invoked for " + name);

    auto results = this->pokemonRepository_->querySearchPokemon(name);

    if (results.empty())
    {
        return Json::Value{};
    }

    return this->pokemonButtonDataToJsonValue(results);
}

Json::Value Pokedex::getLatestsPokemon()
{
    this->logger_.debug("Pokedex::getLatestsPokemon invoked");

    // Grab the amount of latest Pokemon from the env variable
    std::uint32_t limit = this->getLimitEnvVar(true);

    auto results = this->pokemonRepository_->queryLatestsPokemon(limit);

    if (results.empty())
    {
        return Json::Value{};
    }

    return this->pokemonButtonDataToJsonValue(results);
}

Json::Value Pokedex::getPokemonData(const std::string& pokedexNumber,
                                    const std::string& regionalFormId)
{
    this->logger_.debug("Pokedex::getPokemonData invoked for Pokedex Number: " + pokedexNumber +
                        " and Regional Form: " + regionalFormId);

    ConnectorFunction connectorFunction = [this](std::vector<EvolutionData>& evolutionaryChain)
    { this->connectEvolutionaryChain(evolutionaryChain); };

    ButtonFunction buttonFunction = [this](const std::vector<std::vector<std::string>>& results)
    { return this->pokemonButtonDataToJsonValue(results); };

    return this->pokemonRepository_->queryPokemon(pokedexNumber, regionalFormId, connectorFunction,
                                                  buttonFunction);
}

Json::Value Pokedex::getPokemonTypes() const
{
    this->logger_.debug("Pokedex::getPokemonTypes invoked");

    return this->typeRepository_->getTypes();
}

Json::Value Pokedex::getRegionData() const
{
    this->logger_.debug("Pokedex::getRegionData invoked");

    return this->regionRepository_->getRegions();
}

Json::Value Pokedex::getPokemonGames() const
{
    this->logger_.debug("Pokedex::getGames invoked");

    return this->gameRepository_->getGames();
}

Json::Value Pokedex::getPokemonMainlineGames() const
{
    this->logger_.debug("Pokedex::getPokemonMainlineGames invoked");

    return this->gameRepository_->getMainlineGames();
}

Json::Value Pokedex::pokemonButtonDataToJsonValue(
    const std::vector<std::vector<std::string>>& results)
{
    Json::Value pokemonButtons(Json::arrayValue);

    this->logger_.debug("Pokedex::pokemonButtonDataToJsonValue invoked");

    try
    {
        for (const auto& row : results)
        {
            Json::Value pokemon(Json::objectValue);

            pokemon["pokedex_number"] = row.at(0);
            pokemon["region_id"]      = row.at(1);
            pokemon["name"]           = row.at(2);
            pokemon["image"]          = row.at(3);

            // Apply the region form to the name if present
            std::string regionId = pokemon["region_id"].asString();
            if (regionId != "NULL")
            {
                // Append the name of the regional form to the Pokemon's name
                pokemon["name"] = Json::Value(
                    this->regionRepository_->queryRegionalFormTable(std::stoi(regionId)) + " " +
                    pokemon["name"].asString());
            }

            logger_.debug("Pokedex::pokemonButtonDataToJsonValue Found: " +
                          pokemon["name"].asString());

            pokemonButtons.append(pokemon);
        }
    }
    catch (const std::invalid_argument& e)
    {
        this->logger_.critical("Pokedex::pokemonButtonDataToJsonValue caught an exception: " +
                               std::string(e.what()));
        return Json::Value(Json::arrayValue);
    }

    return pokemonButtons;
}

std::uint32_t Pokedex::getLimitEnvVar(const bool& latest) const
{
    std::uint32_t limit = 1;
    std::string envVar;

    this->logger_.debug("Pokedex::getLimitEnvVar invoked");

    try
    {
        if (latest)
        {
            envVar = Config::getInstance().get("LATEST_LIMIT");
            this->logger_.debug("Found the latest limit environment variable: " + envVar);
        }
        else
        {
            envVar = Config::getInstance().get("STARTING_LIMIT");
            this->logger_.debug("Found the starting limit environment variable: " + envVar);
        }

        limit = std::stoi(envVar);
    }
    catch (const std::exception& e)
    {
        this->logger_.critical("Pokedex::getLimitEnvVar caught an exception: " +
                               std::string(e.what()));
    }

    return limit;
}

void Pokedex::connectEvolutionaryChain(std::vector<EvolutionData>& evolutionaryChain) const
{
    this->logger_.debug("Pokedex::connectEvolutionaryChain invoked");

    // Map to store depth values for sorting
    std::unordered_map<std::pair<std::uint32_t, std::uint32_t>, int, pair_hash> depthMap;

    // Define assignDepth as a std::function to allow recursion
    std::function<void(const EvolutionData&, int)> assignDepth =
        [&](const EvolutionData& data, int depth)
    {
        auto key = std::make_pair(data.basePokedexNumber, data.baseRegionId);
        if (depthMap.find(key) == depthMap.end())
        {
            depthMap[key] = depth;

            // Look for evolutions starting from this Pokémon
            for (const auto& evolution : evolutionaryChain)
            {
                if (evolution.basePokedexNumber == data.evolvedPokedexNumber &&
                    evolution.baseRegionId == data.evolvedRegionId)
                {
                    assignDepth(evolution, depth + 1);
                }
            }
        }
    };

    // Start assigning depths from Pokémon with no predecessors
    for (const auto& data : evolutionaryChain)
    {
        auto key = std::make_pair(data.basePokedexNumber, data.baseRegionId);
        if (std::none_of(evolutionaryChain.begin(), evolutionaryChain.end(),
                         [&](const EvolutionData& other)
                         {
                             return other.evolvedPokedexNumber == data.basePokedexNumber &&
                                    other.evolvedRegionId == data.baseRegionId;
                         }))
        {
            assignDepth(data, 0);  // Start from depth 0
        }
    }

    // Sort the evolutionary chain by depth, prioritizing pre-evolutions and regional splits
    std::sort(evolutionaryChain.begin(), evolutionaryChain.end(),
              [&](const EvolutionData& a, const EvolutionData& b)
              {
                  auto keyA = std::make_pair(a.basePokedexNumber, a.baseRegionId);
                  auto keyB = std::make_pair(b.basePokedexNumber, b.baseRegionId);

                  // Primary sorting by depth
                  if (depthMap[keyA] != depthMap[keyB]) return depthMap[keyA] < depthMap[keyB];

                  // Secondary sorting: prioritize non-regional forms over regional forms
                  if (a.baseRegionId != b.baseRegionId) return a.baseRegionId < b.baseRegionId;

                  // Tertiary sorting: prioritize evolved regional forms consistently
                  if (a.evolvedRegionId != b.evolvedRegionId)
                      return a.evolvedRegionId < b.evolvedRegionId;

                  // Final fallback: order by Pokédex numbers
                  return a.evolvedPokedexNumber < b.evolvedPokedexNumber;
              });
}
