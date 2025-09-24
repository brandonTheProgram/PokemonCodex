
#include "repositories/PokemonRepository.h"
#include "repositories/RegionRepository.h"
#include "repositories/TypeRepository.h"
#include "repositories/AttackRepository.h"
#include "SQLManager.h"

PokemonRepository::PokemonRepository(SQLManager& sqlManager, RegionRepository& regionRepository, TypeRepository& typeRepository, AttackRepository& attackRepository) : Respository(sqlManager), regionRepository_(regionRepository), typeRepository_(typeRepository), attackRepository_(attackRepository) {}

Json::Value PokemonRepository::queryPokemon(const std::string& pokedexNumber, const std::string& regionalFormId, ConnectorFunction connectorFunction, ButtonFunction buttonFunction)
{
    Json::Value targetPokemon(Json::objectValue);

    this->logger_.debug("PokemonRepository::queryPokemon invoked for Pokedex Number: " + pokedexNumber +
                        " and Regional Form: " + regionalFormId);

    this->sqlManager_.prepareStatement(
        "SELECT * FROM Pokemon WHERE pokedex_number = ? AND region_id IS ?;");
    this->sqlManager_.bind(1, pokedexNumber);

    try
    {
        if (regionalFormId.empty())
        {
            this->sqlManager_.bind(2, nullptr);
        }
        else
        {
            this->sqlManager_.bind(2, std::stoi(regionalFormId));
        }

        auto results = this->sqlManager_.fetchResults();

        if (results.empty())
        {
            this->logger_.warning("No Pokemon was found that matches the Pokedex Number " +
                                  pokedexNumber);
            return targetPokemon;
        }

        // Popualte variables with info grabbed from the database
        std::string pokedexNumber    = results[0].at(0);
        std::string regionId         = results[0].at(1);
        std::string name             = results[0].at(2);
        std::string image            = results[0].at(3);
        std::string species          = results[0].at(4);
        std::string primaryType      = results[0].at(5);
        std::string secondaryType    = results[0].at(6);
        std::string primaryAbility   = results[0].at(7);
        std::string secondaryAbility = results[0].at(8);
        std::string hiddenAbility    = results[0].at(9);
        std::string description      = results[0].at(10);

        // Modify name based on regional form
        if (!regionalFormId.empty())
        {
            // Append the name of the regional form to the Pokemon's name
            name = this->regionRepository_.queryRegionalFormTable(std::stoi(regionalFormId)) + " " + name;
        }

        // Modify pokemon type(s) into strings and query for the type effectivess based on the
        // values
        if (secondaryType == "NULL")
        {
            targetPokemon["type_effectiveness"] =
                this->typeRepository_.queryTypeEffectivnessTable(std::stoi(primaryType), 0);
            secondaryType = "";
        }
        else
        {
            targetPokemon["type_effectiveness"] =
                this->typeRepository_.queryTypeEffectivnessTable(std::stoi(primaryType), std::stoi(secondaryType));
            secondaryType = this->typeRepository_.queryTypeTable(std::stoi(secondaryType));
        }
        primaryType = this->typeRepository_.queryTypeTable(std::stoi(primaryType));

        // Modify pokemon ability(s) into strings
        primaryAbility = this->queryAbilityTable(std::stoi(primaryAbility));

        if (secondaryAbility == "NULL")
        {
            secondaryAbility = "";
        }
        else
        {
            secondaryAbility = this->queryAbilityTable(std::stoi(secondaryAbility));
        }

        if (hiddenAbility == "NULL")
        {
            hiddenAbility = "";
        }
        else
        {
            hiddenAbility = this->queryAbilityTable(std::stoi(hiddenAbility));
        }

        targetPokemon["pokedex_number"]    = pokedexNumber;
        targetPokemon["region_id"]         = regionId;
        targetPokemon["name"]              = name;
        targetPokemon["image"]             = image;
        targetPokemon["species"]           = species;
        targetPokemon["primary_type"]      = primaryType;
        targetPokemon["secondary_type"]    = secondaryType;
        targetPokemon["primary_ability"]   = primaryAbility;
        targetPokemon["secondary_ability"] = secondaryAbility;
        targetPokemon["hidden_ability"]    = hiddenAbility;
        targetPokemon["description"]       = description;
        targetPokemon["evolutionary_line"] = this->queryEvolutionTable(std::stoi(pokedexNumber), connectorFunction, buttonFunction);
        targetPokemon["game_locations"] =
            this->queryLocationTable(std::stoi(pokedexNumber), regionalFormId);
        targetPokemon["level_up_moveset"] =
            this->attackRepository_.queryLevelUpMovesetTable(std::stoi(pokedexNumber), regionalFormId);
        auto [tmMoves, trMoves] =
            this->attackRepository_.queryTechnicalMovesetTable(std::stoi(pokedexNumber), regionalFormId);
        targetPokemon["technical_machines"] = tmMoves;
        targetPokemon["technical_records"]  = trMoves;
    }
    catch (const std::invalid_argument& e)
    {
        this->logger_.critical("Pokedex::getPokemonData caught an exception: " +
                               std::string(e.what()));
        return Json::Value(Json::objectValue);
    }

    return targetPokemon;
}

std::vector<Json::Value::Members> PokemonRepository::queryRegionPokemon(const std::string& region, const bool shouldLimit, const uint32_t& limit)
{
    this->logger_.debug("PokemonRepository::queryRegionPokemon invoked for " + region);

    if(!this->regionRepository_.regionExists(region))
    {
         this->logger_.warning("The following region is not valid: " + region);
        return {};
    }

    // Get the respective region range
    auto endpoints = this->regionRepository_.getEndpoints(region);

    // Get the latest region range
    std::uint32_t start = endpoints.first; 
    std::uint32_t end   = endpoints.second; 

    // Grab the Pokemon from the respective region
    this->logger_.info("PokemonRepository::queryRegionPokemon Grabbing the Pokemon from the " + region +
                       " region");
    this->sqlManager_.prepareStatement(
        "SELECT pokedex_number, region_id, name, image FROM Pokemon WHERE pokedex_number BETWEEN ? "
        "AND ?;");
    this->sqlManager_.bind(1, start);

    // Grab the start of the region for the homepage
    if (shouldLimit)
    {
        if (((end - start) - limit) >= 0)
        {
            this->sqlManager_.bind(2, start + limit - 1);
        }
        else
        {
            this->sqlManager_.bind(2, start + 1);
        }
    }
    else
    {
        this->sqlManager_.bind(2, end);
    }

    auto results = this->sqlManager_.fetchResults();

    if (shouldLimit && results.size() > limit)
    {
        results.resize(limit);
    }

    return results;
}

std::vector<Json::Value::Members> PokemonRepository::querySearchPokemon(const std::string& name)
{
    this->logger_.debug("PokemonRepository::querySearchPokemon invoked for " + name);

    if (name.empty())
    {
        this->logger_.warning("The user entered an empty Pokemon name into the search bar");
        return {};
    }

    // Grab the Pokemon from the respective region
    this->logger_.info("PokemonRepository::querySearchPokemon Searching for the Pokemon named " + name);
    this->sqlManager_.prepareStatement(
        "SELECT pokedex_number, region_id, name, image FROM Pokemon WHERE name LIKE ?;");
    this->sqlManager_.bind(1, name + '%');

    return this->sqlManager_.fetchResults();
}

std::vector<Json::Value::Members> PokemonRepository::queryLatestsPokemon(const std::uint32_t& limit)
{
    this->logger_.debug("PokemonRepository::queryLatestsPokemon invoked");

    if(!this->regionRepository_.regionExists("All"))
    {
        this->logger_.warning("The database is missing a region called 'All'");
        return {};
    }

    auto endpoints = this->regionRepository_.getEndpoints("All");

    // Get the latest region range
    std::uint32_t start = endpoints.first; 
    std::uint32_t end   = endpoints.second; 

    // Grab the latest Pokemon
    if (limit < start || limit > end)
    {
        this->logger_.warning("The limit of " + std::to_string(limit) +
                              " is out of range. The latests Pokemon will be limited to 1");
        this->sqlManager_.prepareStatement(
            "SELECT pokedex_number, region_id, name, image FROM Pokemon ORDER BY pokedex_number "
            "DESC LIMIT 1;");
    }
    else
    {
        this->logger_.info("PokemonRepository::queryLatestsPokemon Grabbing the latests " +
                           std::to_string(limit) + " Pokemon");
        this->sqlManager_.prepareStatement(
            "SELECT pokedex_number, region_id, name, image FROM Pokemon ORDER BY pokedex_number "
            "DESC LIMIT ?;");
        this->sqlManager_.bind(1, limit);
    }

    return this->sqlManager_.fetchResults();
}

Json::Value PokemonRepository::queryLocationTable(const std::uint32_t& pokedexNumber, const std::string& regionalFormId)
{
    this->logger_.debug("PokemonRepository::queryLocationTable invoked to query: " +
                        std::to_string(pokedexNumber) + " and " + regionalFormId);

    Json::Value pokemonLocations(Json::arrayValue);

    this->sqlManager_.prepareStatement(
        "SELECT location_name FROM Pokemon_Location WHERE pokedex_number = ? AND region_id IS ?;");
    this->sqlManager_.bind(1, pokedexNumber);

    try
    {
        if (regionalFormId.empty())
        {
            this->sqlManager_.bind(2, nullptr);
        }
        else
        {
            this->sqlManager_.bind(2, std::stoi(regionalFormId));
        }

        auto results = this->sqlManager_.fetchResults();

        if (results.empty())
        {
            this->logger_.warning("No Pokemon was found that matches the Pokedex Number " +
                                  pokedexNumber);
            return pokemonLocations;
        }

        for (const auto& row : results)
        {
            std::string pokemonLocation = row[0];

            if (pokemonLocation == "NULL")
            {
                pokemonLocations.append(
                    Json::Value("This Pokémon is not found in the wild in this game"));
            }
            else
            {
                this->logger_.debug("PokemonRepository::queryLocationTable found the location of: " +
                                    pokemonLocation);

                pokemonLocations.append(Json::Value(pokemonLocation));
            }
        }
    }
    catch (const std::invalid_argument& e)
    {
        this->logger_.critical("PokemonRepository::queryLocationTable caught an exception: " +
                               std::string(e.what()));
        return Json::Value(Json::arrayValue);
    }

    return pokemonLocations;
}

std::string PokemonRepository::queryAbilityTable(const std::uint32_t& id)
{
    this->logger_.debug("PokemonRepository::queryAbilityTable invoked to query: " + std::to_string(id));

    std::string ability = "";

    this->sqlManager_.prepareStatement(
        "SELECT name, description FROM Pokemon_Ability WHERE ability_id = ?;");
    this->sqlManager_.bind(1, id);
    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        this->logger_.warning("No ability was found that matches the id of " + std::to_string(id));
        return ability;
    }

    return Json::Value(results[0].at(0)).asString() + ": " +
           Json::Value(results[0].at(1)).asString();
}

Json::Value PokemonRepository::queryEvolutionTable(const std::uint32_t& targetPokedexNumber, ConnectorFunction connectorFunction, ButtonFunction buttonFunction)
{
    this->logger_.debug("PokemonRepository::queryEvolutionTable invoked to query: " +
                        std::to_string(targetPokedexNumber));

    Json::Value evolutionList(Json::arrayValue);

    this->sqlManager_.prepareStatement(
        "SELECT base_pokedex_number, evolved_pokedex_number, base_region_id, evolved_region_id, "
        "evolution_condition "
        "FROM Pokemon_Evolution "
        "WHERE chain_id = (SELECT chain_id FROM Pokemon_Evolution WHERE base_pokedex_number = ? OR "
        "evolved_pokedex_number = ? LIMIT 1);");

    this->sqlManager_.bind(1, targetPokedexNumber);
    this->sqlManager_.bind(2, targetPokedexNumber);

    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        this->logger_.debug("No Pokemon Evolution Line was found for the provided Pokemon: " +
                            std::to_string(targetPokedexNumber));
        return evolutionList;
    }

    std::vector<EvolutionData> evolutionData;

    try
    {
        for (const auto& row : results)
        {
            std::uint32_t basePokedexNumber    = std::stoi(row[0]);
            std::uint32_t evolvedPokedexNumber = std::stoi(row[1]);
            std::uint32_t baseRegionId         = (row[2] == "NULL") ? 0 : std::stoi(row[2]);
            std::uint32_t evolvedRegionId      = (row[3] == "NULL") ? 0 : std::stoi(row[3]);
            std::string condition              = row[4];

            evolutionData.emplace_back(basePokedexNumber, evolvedPokedexNumber, baseRegionId,
                                       evolvedRegionId, condition);
        }

        // Connect the evolution data to form an evolutionary chain
        connectorFunction(evolutionData);

        // Convert sorted data into JSON
        for (const auto& data : evolutionData)
        {
            Json::Value entry(Json::objectValue);

            // Fetch data to turn the evolutionary line into buttons
            this->sqlManager_.prepareStatement(
                "SELECT pokedex_number, region_id, name, image FROM Pokemon WHERE pokedex_number = "
                "? AND region_id IS ?;");
            this->sqlManager_.bind(1, data.basePokedexNumber);

            if (data.baseRegionId == 0)
            {
                this->sqlManager_.bind(2, nullptr);
            }
            else
            {
                this->sqlManager_.bind(2, data.baseRegionId);
            }

            auto baseButtonData    = this->sqlManager_.fetchResults();
            Json::Value baseButton = buttonFunction(baseButtonData);

            this->sqlManager_.prepareStatement(
                "SELECT pokedex_number, region_id, name, image FROM Pokemon WHERE pokedex_number = "
                "? AND region_id IS ?;");
            this->sqlManager_.bind(1, data.evolvedPokedexNumber);

            if (data.evolvedRegionId == 0)
            {
                this->sqlManager_.bind(2, nullptr);
            }
            else
            {
                this->sqlManager_.bind(2, data.evolvedRegionId);
            }

            auto evolvedButtonData    = this->sqlManager_.fetchResults();
            Json::Value evolvedButton = buttonFunction(evolvedButtonData);

            // Populate the entry
            entry["base"]                = baseButton[0];
            entry["evolved"]             = evolvedButton[0];
            entry["evolution_condition"] = data.evolutionCondition;

            evolutionList.append(entry);
        }

        Json::StreamWriterBuilder builder;
        builder["indentation"] = "";

        this->logger_.debug("Evolutionary Line: " + Json::writeString(builder, evolutionList));

        return evolutionList;
    }
    catch (const std::invalid_argument& e)
    {
        this->logger_.critical("PokemonRepository::queryEvolutionTable caught an exception: " +
                               std::string(e.what()));
        return Json::Value(Json::arrayValue);
    }
}