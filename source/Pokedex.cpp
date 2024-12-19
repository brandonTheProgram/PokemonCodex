
#include "Pokedex.h"

#include "Config.h"

Pokedex::Pokedex()
    : sqlManager(Config::getInstance().get("DATABASE_PATH")),
      logger(Logger::getInstance()),
      keys(),
      regions()
{
    this->initializeRegions();
    this->initializePokemonTypes();
}

Json::Value Pokedex::getRegionPokemonData(const std::string& region, const bool& shouldLimit)
{
    Json::Value regionData(Json::arrayValue);
    int limit;

    this->logger.debug("Pokedex::getRegionPokemonData invoked for " + region);

    if (this->regions.find(region) == this->regions.end())
    {
        this->logger.warning("The following region is not valid: " + region);
        return Json::Value{};
    }

    // Get the respective region range
    std::uint32_t start = this->regions[region].start;
    std::uint32_t end   = this->regions[region].end;

    // Grab the Pokemon from the respective region
    this->logger.info("Pokedex::getRegionPokemonData Grabbing the Pokemon from the " + region +
                      " region");
    this->sqlManager.prepareStatement(
        "SELECT pokedex_number, region_id, name, image FROM Pokemon WHERE pokedex_number BETWEEN ? "
        "AND ?;");
    this->sqlManager.bind(1, start);

    // Grab the start of the region for the homepage
    if (shouldLimit)
    {
        limit = this->getLimitEnvVar();

        if (((end - start) - limit) >= 0)
        {
            this->sqlManager.bind(2, start + limit - 1);
        }
        else
        {
            this->sqlManager.bind(2, start + 1);
        }
    }
    else
    {
        this->sqlManager.bind(2, end);
    }

    auto results = this->sqlManager.fetchResults();

    if (shouldLimit && results.size() > limit)
    {
        results.resize(limit);
    }

    return this->pokemonButtonDataToJsonValue(results);
}

Json::Value Pokedex::getLatestsPokemon()
{
    Json::Value latestPokemon(Json::arrayValue);

    this->logger.debug("Pokedex::getLatestsPokemon invoked");

    // Grab the amount of latest Pokemon from the env variable
    std::uint32_t limit = this->getLimitEnvVar(true);

    if (this->regions.find("All") == this->regions.end())
    {
        this->logger.warning("The database is missing a region called 'All'");
        return Json::Value{};
    }

    // Get the latest region range
    std::uint32_t start = this->regions["All"].start;
    std::uint32_t end   = this->regions["All"].end;

    // Grab the latest Pokemon
    if (limit < start || limit > end)
    {
        this->logger.warning("The limit of " + std::to_string(limit) +
                             " is out of range. The latests Pokemon will be limited to 1");
        this->sqlManager.prepareStatement(
            "SELECT pokedex_number, region_id, name, image FROM Pokemon ORDER BY pokedex_number "
            "DESC LIMIT 1;");
    }
    else
    {
        this->logger.info("Pokedex::getLatestsPokemon Grabbing the latests " +
                          std::to_string(limit) + " Pokemon");
        this->sqlManager.prepareStatement(
            "SELECT pokedex_number, region_id, name, image FROM Pokemon ORDER BY pokedex_number "
            "DESC LIMIT ?;");
        this->sqlManager.bind(1, limit);
    }

    auto results = this->sqlManager.fetchResults();

    return this->pokemonButtonDataToJsonValue(results);
}

Json::Value Pokedex::getPokemonData(const std::string& pokedexNumber,
                                    const std::string& regionalFormId)
{
    Json::Value targetPokemon(Json::objectValue);

    this->logger.debug("Pokedex::getPokemonData invoked for Pokedex Number: " + pokedexNumber +
                       " and Regional Form: " + regionalFormId);

    // Grab the target Pokemon basic information
    this->sqlManager.prepareStatement(
        "SELECT * FROM Pokemon WHERE pokedex_number = ? AND region_id IS ?;");
    this->sqlManager.bind(1, pokedexNumber);

    try
    {
        if (regionalFormId.empty())
        {
            this->sqlManager.bind(2, nullptr);
        }
        else
        {
            this->sqlManager.bind(2, std::stoi(regionalFormId));
        }

        auto results = this->sqlManager.fetchResults();

        if (results.empty())
        {
            this->logger.warning("No Pokemon was found that matching the Pokedex Number " +
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
            name = this->queryRegionalFormTable(std::stoi(regionalFormId)) + " " + name;
        }

        // Modify pokemon type(s) into strings and query for the type effectivess based on the
        // values
        if (secondaryType == "NULL")
        {
            targetPokemon["type_effectiveness"] =
                this->queryTypeEffectivnessTable(std::stoi(primaryType), 0);
            secondaryType = "";
        }
        else
        {
            targetPokemon["type_effectiveness"] =
                this->queryTypeEffectivnessTable(std::stoi(primaryType), std::stoi(secondaryType));
            secondaryType = this->queryTypeTable(std::stoi(secondaryType));
        }
        primaryType = this->queryTypeTable(std::stoi(primaryType));

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

        // Grab the target Pokemon's evolutionary line

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
    }
    catch (const std::invalid_argument& e)
    {
        this->logger.critical("Pokedex::getPokemonData caught an exception: " +
                              std::string(e.what()));
        return Json::Value(Json::objectValue);
    }

    return targetPokemon;
}

Json::Value Pokedex::getPokemonTypes() const
{
    Json::Value pokemonTypes(Json::arrayValue);

    this->logger.debug("Pokedex::getPokemonTypes invoked");

    for (std::string pokemonType : this->pokemonTypes)
    {
        Json::Value type(Json::objectValue);
        type["name"] = pokemonType;

        pokemonTypes.append(type);
    }

    return pokemonTypes;
}

Json::Value Pokedex::getRegionData() const
{
    Json::Value regionNames(Json::arrayValue);

    this->logger.debug("Pokedex::getRegionData invoked");

    for (std::string key : this->keys)
    {
        Json::Value region(Json::objectValue);
        region["name"]  = key;
        region["image"] = this->regions.at(key).image;

        regionNames.append(region);
    }

    return regionNames;
}

void Pokedex::initializeRegions()
{
    this->logger.debug("Pokedex::initializeRegions invoked");

    this->sqlManager.prepareStatement("SELECT name, image, start, end FROM Pokemon_Region;");
    auto results = this->sqlManager.fetchResults();

    if (results.empty())
    {
        this->logger.critical(
            "No regions were found in the database, verify that Pokemon_Region table is populated "
            "in the database");
        throw std::runtime_error(
            "No regions were found in the database, verify that Pokemon_Region table is populated "
            "in the database");
    }

    try
    {
        for (const auto& row : results)
        {
            std::string name    = Json::Value(row.at(0)).asString();
            std::string image   = Json::Value(row.at(1)).asString();
            std::uint32_t start = std::stoi(Json::Value(row.at(2)).asString());
            std::uint32_t end   = std::stoi(Json::Value(row.at(3)).asString());

            this->regions[row.at(0)] = Region(name, image, start, end);
            this->keys.push_back(name);
        }
    }
    catch (const std::invalid_argument& e)
    {
        this->logger.critical("Pokedex::initializeRegions caught an exception: " +
                              std::string(e.what()));
        throw e;
    }
}

void Pokedex::initializePokemonTypes()
{
    this->logger.debug("Pokedex::initializePokemonTypes invoked");

    this->sqlManager.prepareStatement("SELECT type_name FROM Pokemon_Type;");

    auto results = this->sqlManager.fetchResults();

    if (results.empty())
    {
        this->logger.critical(
            "No Pokemon Types were found in the database, verify that Pokemon_Type table is "
            "populated "
            "in the database");
        throw std::runtime_error(
            "No Pokemon Types were found in the database, verify that Pokemon_Type table is "
            "populated "
            "in the database");
    }

    for (const auto& row : results)
    {
        std::string name = Json::Value(row.at(0)).asString();
        this->pokemonTypes.push_back(name);
    }
}

std::string Pokedex::queryRegionalFormTable(const std::uint32_t& id)
{
    this->logger.debug("Pokedex::queryRegionalFormTable invoked to query: " + std::to_string(id));

    std::string regionalFormName = "";

    this->sqlManager.prepareStatement(
        "SELECT region_name FROM Pokemon_Regional_Form WHERE region_id = ?;");
    this->sqlManager.bind(1, id);
    auto results = this->sqlManager.fetchResults();

    if (results.empty())
    {
        this->logger.warning("No regional form was found that matches the id of " +
                             std::to_string(id));
        return regionalFormName;
    }

    return Json::Value(results[0].at(0)).asString();
}

std::string Pokedex::queryTypeTable(const std::uint32_t& id)
{
    this->logger.debug("Pokedex::queryTypeTable invoked to query: " + std::to_string(id));

    std::string type = "";

    this->sqlManager.prepareStatement("SELECT type_name FROM Pokemon_Type WHERE type_id = ?;");
    this->sqlManager.bind(1, id);
    auto results = this->sqlManager.fetchResults();

    if (results.empty())
    {
        this->logger.warning("No type was found that matches the id of " + std::to_string(id));
        return type;
    }

    return Json::Value(results[0].at(0)).asString();
}

std::string Pokedex::queryAbilityTable(const std::uint32_t& id)
{
    this->logger.debug("Pokedex::queryAbilityTable invoked to query: " + std::to_string(id));

    std::string ability = "";

    this->sqlManager.prepareStatement(
        "SELECT name, description FROM Pokemon_Ability WHERE ability_id = ?;");
    this->sqlManager.bind(1, id);
    auto results = this->sqlManager.fetchResults();

    if (results.empty())
    {
        this->logger.warning("No ability was found that matches the id of " + std::to_string(id));
        return ability;
    }

    return Json::Value(results[0].at(0)).asString() + ": " +
           Json::Value(results[0].at(1)).asString();
}

Json::Value Pokedex::queryTypeEffectivnessTable(const std::uint32_t& primaryTypeId,
                                                const std::uint32_t& secondaryTypeId)
{
    this->logger.debug("Pokedex::queryTypeEffectivnessTable invoked to query: " +
                       std::to_string(primaryTypeId) + " and " + std::to_string(secondaryTypeId));

    Json::Value pokemonTypeEffective(Json::arrayValue);

    this->sqlManager.prepareStatement(
        "SELECT defending_type_id, attacking_type_id, damage_multiplier "
        "FROM Pokemon_Type_Effectivness "
        "WHERE attacking_type_id IN (?, ?);");
    this->sqlManager.bind(1, primaryTypeId);
    this->sqlManager.bind(2, secondaryTypeId);
    auto results = this->sqlManager.fetchResults();

    if (results.empty())
    {
        this->logger.warning("No type effectiveness was found for the provided type IDs: " +
                             std::to_string(primaryTypeId) + " and " +
                             std::to_string(secondaryTypeId));
        return pokemonTypeEffective;
    }

    // Create a map to store effectiveness grouped by defending_type_id
    std::unordered_map<std::uint32_t, double> effectivenessMap;

    try
    {
        // Process the results for both primary and secondary types
        for (const auto& row : results)
        {
            std::uint32_t defendingTypeId = static_cast<std::uint32_t>(std::stoul(row[0]));
            std::uint32_t attackingTypeId = static_cast<std::uint32_t>(std::stoul(row[1]));
            double damageMultiplier       = std::stod(row[2]);

            if (attackingTypeId == primaryTypeId)
            {
                // Initialize with primary type damage multipliers
                effectivenessMap[defendingTypeId] = damageMultiplier;
            }
            else if (attackingTypeId == secondaryTypeId)
            {
                // Combine with existing primary type multiplier (if present)
                if (effectivenessMap.find(defendingTypeId) != effectivenessMap.end())
                {
                    effectivenessMap[defendingTypeId] *= damageMultiplier;
                }
            }
        }

        for (auto i : effectivenessMap)
        {
            Json::Value typeEffectiveness;
            typeEffectiveness["damage_multiplier"] = i.second;
            pokemonTypeEffective.append(typeEffectiveness);
        }
    }
    catch (const std::invalid_argument& e)
    {
        this->logger.critical("Pokedex::queryTypeEffectivnessTable caught an exception: " +
                              std::string(e.what()));
        return Json::Value(Json::arrayValue);
    }

    return pokemonTypeEffective;
}

Json::Value Pokedex::pokemonButtonDataToJsonValue(
    const std::vector<std::vector<std::string>>& results)
{
    Json::Value pokemonButtons(Json::arrayValue);

    this->logger.debug("Pokedex::pokemonButtonDataToJsonValue invoked");

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
                pokemon["name"] = Json::Value(this->queryRegionalFormTable(std::stoi(regionId)) +
                                              " " + pokemon["name"].asString());
            }

            logger.debug("Pokedex::pokemonButtonDataToJsonValue Found: " +
                         pokemon["name"].asString());

            pokemonButtons.append(pokemon);
        }
    }
    catch (const std::invalid_argument& e)
    {
        this->logger.critical("Pokedex::pokemonButtonDataToJsonValue caught an exception: " +
                              std::string(e.what()));
        return Json::Value(Json::arrayValue);
    }

    return pokemonButtons;
}

std::uint32_t Pokedex::getLimitEnvVar(const bool& latest) const
{
    std::uint32_t limit = 1;
    std::string envVar;

    try
    {
        if (latest)
        {
            envVar = Config::getInstance().get("LATEST_LIMIT");
            this->logger.debug("Found the latest limit environment variable: " + envVar);
        }
        else
        {
            envVar = Config::getInstance().get("STARTING_LIMIT");
            this->logger.debug("Found the starting limit environment variable: " + envVar);
        }

        limit = std::stoi(envVar);
    }
    catch (const std::exception& e)
    {
        this->logger.critical("Pokedex::getLimitEnvVar caught an exception: " +
                              std::string(e.what()));
    }

    return limit;
}