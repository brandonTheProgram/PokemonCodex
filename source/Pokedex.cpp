
#include "Pokedex.h"

#include "Config.h"

Pokedex::Pokedex()
    : sqlManager(Config::getInstance().get("DATABASE_PATH")),
      logger(Logger::getInstance()),
      keys(),
      regions()
{
    this->initializeRegions();
}

Json::Value Pokedex::getRegionPokemonData(const std::string& region, const bool& shouldLimit)
{
    Json::Value regionData(Json::arrayValue);
    int limit;

    this->logger.debug("Pokedex::getRegionPokemonData invoked");

    if (this->regions.find(region) == this->regions.end())
    {
        this->logger.warning("The following region is not valid: " + region);
        return Json::Value{};
    }

    // Get the respective region range
    std::uint32_t start = this->regions[region].start;
    std::uint32_t end = this->regions[region].end;

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
    std::uint32_t end = this->regions["All"].end;

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

Json::Value Pokedex::getRegionData() const
{
    Json::Value regionNames(Json::arrayValue);

    this->logger.debug("Pokedex::getRegionData invoked");

    for (std::string key : this->keys)
    {
        Json::Value region(Json::objectValue);
        region["name"] = key;
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

    for (const auto& row : results)
    {
        std::string name = Json::Value(row.at(0)).asString();
        std::string image = Json::Value(row.at(1)).asString();
        std::uint32_t start = std::stoi(Json::Value(row.at(2)).asString());
        std::uint32_t end = std::stoi(Json::Value(row.at(3)).asString());

        this->regions[row.at(0)] = Region(name, image, start, end);
        this->keys.push_back(name);
    }
}

std::string Pokedex::getRegionalFormName(const std::uint32_t& id)
{
    this->logger.debug("Pokedex::getRegionalFormName invoked");

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

Json::Value Pokedex::pokemonButtonDataToJsonValue(
    const std::vector<std::vector<std::string>>& results)
{
    Json::Value pokemonButtons(Json::arrayValue);

    this->logger.debug("Pokedex::pokemonButtonDataToJsonValue invoked");

    for (const auto& row : results)
    {
        Json::Value pokemon(Json::objectValue);

        pokemon["pokedex_number"] = row.at(0);
        pokemon["region_id"] = row.at(1);
        pokemon["name"] = row.at(2);
        pokemon["image"] = row.at(3);

        // Apply the region form to the name if present
        std::string regionId = pokemon["region_id"].asString();
        if (regionId != "NULL")
        {
            // Append the name of the regional form to the Pokemon's name
            pokemon["name"] = Json::Value(this->getRegionalFormName(std::stoi(regionId)) + " " +
                                          pokemon["name"].asString());
        }

        logger.debug("Pokedex::pokemonButtonDataToJsonValue Found: " + pokemon["name"].asString());

        pokemonButtons.append(pokemon);
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
        this->logger.warning("Pokedex::getLimitEnvVar: " + std::string(e.what()));
    }

    return limit;
}