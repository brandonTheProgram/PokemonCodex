
#include "Pokedex.h"

#include <functional>
#include <map>

#include "Config.h"

Pokedex::Pokedex()
    : sqlManager_(Config::getInstance().get("DATABASE_PATH")),
      logger_(Logger::getInstance()),
      keys_(),
      pokemonTypes_(),
      pokemonGames_(),
      pokemonMainlineGames_(),
      regions_()
{
    this->initializeRegions();
    this->initializePokemonTypes();
    this->initializeGames();
    this->initializeMainlineGames();
}

Json::Value Pokedex::getRegionPokemonData(const std::string& region, const bool shouldLimit)
{
    int limit;

    this->logger_.debug("Pokedex::getRegionPokemonData invoked for " + region);

    if (this->regions_.find(region) == this->regions_.end())
    {
        this->logger_.warning("The following region is not valid: " + region);
        return Json::Value{};
    }

    // Get the respective region range
    std::uint32_t start = this->regions_[region].start;
    std::uint32_t end   = this->regions_[region].end;

    // Grab the Pokemon from the respective region
    this->logger_.info("Pokedex::getRegionPokemonData Grabbing the Pokemon from the " + region +
                       " region");
    this->sqlManager_.prepareStatement(
        "SELECT pokedex_number, region_id, name, image FROM Pokemon WHERE pokedex_number BETWEEN ? "
        "AND ?;");
    this->sqlManager_.bind(1, start);

    // Grab the start of the region for the homepage
    if (shouldLimit)
    {
        limit = this->getLimitEnvVar();

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

    return this->pokemonButtonDataToJsonValue(results);
}

Json::Value Pokedex::getSearchPokemon(const std::string& name)
{
    this->logger_.debug("Pokedex::getSearchPokemon invoked for " + name);

    if (name.empty())
    {
        this->logger_.warning("The user entered an empty Pokemon name into the serach bar");
        return Json::Value{};
    }

    // Grab the Pokemon from the respective region
    this->logger_.info("Pokedex::getRegionPokemonData Searching for the Pokemon named " + name);
    this->sqlManager_.prepareStatement(
        "SELECT pokedex_number, region_id, name, image FROM Pokemon WHERE name LIKE ?;");
    this->sqlManager_.bind(1, name + '%');

    auto results = this->sqlManager_.fetchResults();

    return this->pokemonButtonDataToJsonValue(results);
}

Json::Value Pokedex::getLatestsPokemon()
{
    Json::Value latestPokemon(Json::arrayValue);

    this->logger_.debug("Pokedex::getLatestsPokemon invoked");

    // Grab the amount of latest Pokemon from the env variable
    std::uint32_t limit = this->getLimitEnvVar(true);

    if (this->regions_.find("All") == this->regions_.end())
    {
        this->logger_.warning("The database is missing a region called 'All'");
        return Json::Value{};
    }

    // Get the latest region range
    std::uint32_t start = this->regions_["All"].start;
    std::uint32_t end   = this->regions_["All"].end;

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
        this->logger_.info("Pokedex::getLatestsPokemon Grabbing the latests " +
                           std::to_string(limit) + " Pokemon");
        this->sqlManager_.prepareStatement(
            "SELECT pokedex_number, region_id, name, image FROM Pokemon ORDER BY pokedex_number "
            "DESC LIMIT ?;");
        this->sqlManager_.bind(1, limit);
    }

    auto results = this->sqlManager_.fetchResults();

    return this->pokemonButtonDataToJsonValue(results);
}

Json::Value Pokedex::getPokemonData(const std::string& pokedexNumber,
                                    const std::string& regionalFormId)
{
    Json::Value targetPokemon(Json::objectValue);

    this->logger_.debug("Pokedex::getPokemonData invoked for Pokedex Number: " + pokedexNumber +
                        " and Regional Form: " + regionalFormId);

    // Grab the target Pokemon basic information
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
        targetPokemon["evolutionary_line"] = this->queryEvolutionTable(std::stoi(pokedexNumber));
        targetPokemon["game_locations"] =
            this->queryLocationTable(std::stoi(pokedexNumber), regionalFormId);
        targetPokemon["level_up_moveset"] =
            this->queryLevelUpMovesetTable(std::stoi(pokedexNumber), regionalFormId);
        auto [tmMoves, trMoves] =
            this->queryTechnicalMovesetTable(std::stoi(pokedexNumber), regionalFormId);
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

Json::Value Pokedex::getPokemonTypes() const
{
    Json::Value pokemonTypesJson(Json::arrayValue);

    this->logger_.debug("Pokedex::getPokemonTypes invoked");

    for (std::string pokemonType : this->pokemonTypes_)
    {
        Json::Value type(Json::objectValue);
        type["name"] = pokemonType;

        pokemonTypesJson.append(type);
    }

    return pokemonTypesJson;
}

Json::Value Pokedex::getRegionData() const
{
    Json::Value regionNames(Json::arrayValue);

    this->logger_.debug("Pokedex::getRegionData invoked");

    for (std::string key : this->keys_)
    {
        Json::Value region(Json::objectValue);
        region["name"]  = key;
        region["image"] = this->regions_.at(key).image;

        regionNames.append(region);
    }

    return regionNames;
}

Json::Value Pokedex::getPokemonGames() const
{
    Json::Value pokemonGamesJson(Json::arrayValue);

    this->logger_.debug("Pokedex::getGames invoked");

    for (std::string pokemonGame : this->pokemonGames_)
    {
        Json::Value game(Json::objectValue);
        game["name"] = pokemonGame;

        pokemonGamesJson.append(game);
    }

    return pokemonGamesJson;
}

Json::Value Pokedex::getPokemonMainlineGames() const
{
    Json::Value pokemonMainlineGamesJson(Json::arrayValue);

    this->logger_.debug("Pokedex::getPokemonMainlineGames invoked");

    for (const auto& pair : this->pokemonMainlineGames_)
    {
        Json::Value game(Json::objectValue);
        game["id"]   = pair.first;
        game["name"] = pair.second;

        pokemonMainlineGamesJson.append(game);
    }

    return pokemonMainlineGamesJson;
}

void Pokedex::initializeRegions()
{
    this->logger_.debug("Pokedex::initializeRegions invoked");

    this->sqlManager_.prepareStatement("SELECT name, image, start, end FROM Pokemon_Region;");
    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        std::string message =
            "No regions were found in the database, verify that Pokemon_Region table is populated "
            "in the database";
        this->logger_.critical(message);
        throw std::runtime_error(message);
    }

    try
    {
        for (const auto& row : results)
        {
            std::string name    = Json::Value(row.at(0)).asString();
            std::string image   = Json::Value(row.at(1)).asString();
            std::uint32_t start = std::stoi(Json::Value(row.at(2)).asString());
            std::uint32_t end   = std::stoi(Json::Value(row.at(3)).asString());

            this->regions_[row.at(0)] = Region(name, image, start, end);
            this->keys_.push_back(name);
        }
    }
    catch (const std::invalid_argument& e)
    {
        this->logger_.critical("Pokedex::initializeRegions caught an exception: " +
                               std::string(e.what()));
        throw e;
    }

    this->logger_.debug("Pokedex::initializeRegions Found " + std::to_string(this->keys_.size()) +
                        " Pokemon Regions");
}

void Pokedex::initializePokemonTypes()
{
    this->logger_.debug("Pokedex::initializePokemonTypes invoked");

    this->sqlManager_.prepareStatement("SELECT type_name FROM Pokemon_Type;");

    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        std::string message =
            "No Pokemon Types were found in the database, verify that Pokemon_Type table is "
            "populated in the database";
        this->logger_.critical(message);
        throw std::runtime_error(message);
    }

    for (const auto& row : results)
    {
        std::string name = Json::Value(row.at(0)).asString();
        this->pokemonTypes_.push_back(name);
    }

    this->logger_.debug("Pokedex::initializePokemonTypes Found " +
                        std::to_string(this->pokemonTypes_.size()) + " Pokemon Types");
}

void Pokedex::initializeGames()
{
    this->logger_.debug("Pokedex::initializeGames invoked");

    this->sqlManager_.prepareStatement("SELECT game_name FROM Pokemon_Game;");

    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        std::string message =
            "No Pokemon Games were found in the database, verify that Pokemon_Game table is "
            "populated in the database";
        this->logger_.critical(message);
        throw std::runtime_error(message);
    }

    for (const auto& row : results)
    {
        std::string name = Json::Value(row.at(0)).asString();
        this->pokemonGames_.push_back(name);
    }

    this->logger_.debug("Pokedex::initializeGames Found " +
                        std::to_string(this->pokemonGames_.size()) + " Pokemon Games");
}

void Pokedex::initializeMainlineGames()
{
    this->logger_.debug("Pokedex::initializeMainlineGames invoked");

    this->sqlManager_.prepareStatement(
        "SELECT mainline_game_id, game_name FROM Pokemon_Mainline_Game ORDER BY mainline_game_id "
        "ASC;");

    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        std::string message =
            "No Mainline Pokemon Games were found in the database, verify that "
            "Pokemon_Mainline_Game table is "
            "populated in the database";
        this->logger_.critical(message);
        throw std::runtime_error(message);
    }

    for (const auto& row : results)
    {
        std::string id   = Json::Value(row.at(0)).asString();
        std::string name = Json::Value(row.at(1)).asString();
        this->pokemonMainlineGames_.insert({id, name});
    }

    this->logger_.debug("Pokedex::initializeMainLineGames Found " +
                        std::to_string(this->pokemonMainlineGames_.size()) +
                        " Mainline Pokemon Games");
}

std::string Pokedex::queryRegionalFormTable(const std::uint32_t& id)
{
    this->logger_.debug("Pokedex::queryRegionalFormTable invoked to query: " + std::to_string(id));

    std::string regionalFormName = "";

    this->sqlManager_.prepareStatement(
        "SELECT region_name FROM Pokemon_Regional_Form WHERE region_id = ?;");
    this->sqlManager_.bind(1, id);
    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        this->logger_.warning("No regional form was found that matches the id of " +
                              std::to_string(id));
        return regionalFormName;
    }

    return Json::Value(results[0].at(0)).asString();
}

std::string Pokedex::queryTypeTable(const std::uint32_t& id)
{
    this->logger_.debug("Pokedex::queryTypeTable invoked to query: " + std::to_string(id));

    std::string type = "";

    this->sqlManager_.prepareStatement("SELECT type_name FROM Pokemon_Type WHERE type_id = ?;");
    this->sqlManager_.bind(1, id);
    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        this->logger_.warning("No type was found that matches the id of " + std::to_string(id));
        return type;
    }

    return Json::Value(results[0].at(0)).asString();
}

std::string Pokedex::queryAbilityTable(const std::uint32_t& id)
{
    this->logger_.debug("Pokedex::queryAbilityTable invoked to query: " + std::to_string(id));

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

std::string Pokedex::queryMoveCategoryTable(const std::uint32_t& id)
{
    this->logger_.debug("Pokedex::queryMoveCategoryTable invoked to query: " + std::to_string(id));

    std::string moveCategory = "";

    this->sqlManager_.prepareStatement(
        "SELECT category_name FROM Pokemon_Move_Category WHERE category_id = ?;");
    this->sqlManager_.bind(1, id);
    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        this->logger_.warning("No move category was found that matches the id of " +
                              std::to_string(id));
        return moveCategory;
    }

    return results[0].at(0);
}

Json::Value Pokedex::queryMoveTable(const std::uint32_t& id)
{
    this->logger_.debug("Pokedex::queryMoveTable invoked to query: " + std::to_string(id));

    Json::Value pokemonMove(Json::objectValue);

    this->sqlManager_.prepareStatement(
        "SELECT move_name, description, type_id, category_id, power, accuracy, pp FROM "
        "Pokemon_Move WHERE move_id = ?;");
    this->sqlManager_.bind(1, id);
    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        this->logger_.warning("No move was found that matches the id of " + std::to_string(id));
        return pokemonMove;
    }

    pokemonMove["name"]        = results[0].at(0);
    pokemonMove["description"] = results[0].at(1);
    pokemonMove["type"]        = this->queryTypeTable(std::stoi(results[0].at(2)));
    pokemonMove["category"]    = this->queryMoveCategoryTable(std::stoi(results[0].at(3)));
    pokemonMove["power"]       = results[0].at(4);
    pokemonMove["accuracy"]    = results[0].at(5);
    pokemonMove["pp"]          = results[0].at(6);

    return pokemonMove;
}

Json::Value Pokedex::queryTechnicalMoveTable(const std::uint32_t& id)
{
    this->logger_.debug("Pokedex::queryTechnicalMoveTable invoked to query: " + std::to_string(id));

    Json::Value pokemonTechnicalMove(Json::objectValue);

    this->sqlManager_.prepareStatement(
        "SELECT move_id, technical_number, is_tr FROM "
        "Pokemon_Technical_Move WHERE technical_move_id = ?;");
    this->sqlManager_.bind(1, id);
    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        this->logger_.warning("No move was found that matches the id of " + std::to_string(id));
        return pokemonTechnicalMove;
    }

    auto moveId = std::stoi(results[0].at(0));

    pokemonTechnicalMove["move"]             = this->queryMoveTable(moveId);
    pokemonTechnicalMove["technical_number"] = results[0].at(1);
    pokemonTechnicalMove["is_tr"]            = results[0].at(2);

    return pokemonTechnicalMove;
}

Json::Value Pokedex::queryTypeEffectivnessTable(const std::uint32_t& primaryTypeId,
                                                const std::uint32_t& secondaryTypeId)
{
    this->logger_.debug("Pokedex::queryTypeEffectivnessTable invoked to query: " +
                        std::to_string(primaryTypeId) + " and " + std::to_string(secondaryTypeId));

    Json::Value pokemonTypeEffective(Json::arrayValue);

    this->sqlManager_.prepareStatement(
        "SELECT defending_type_id, attacking_type_id, damage_multiplier "
        "FROM Pokemon_Type_Effectiveness "
        "WHERE attacking_type_id IN (?, ?)  ORDER BY defending_type_id ASC;");
    this->sqlManager_.bind(1, primaryTypeId);
    this->sqlManager_.bind(2, secondaryTypeId);
    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        this->logger_.warning("No type effectiveness was found for the provided type IDs: " +
                              std::to_string(primaryTypeId) + " and " +
                              std::to_string(secondaryTypeId));
        return pokemonTypeEffective;
    }

    // Create a map to store effectiveness grouped by defending_type_id
    std::map<std::uint32_t, double> effectivenessMap;

    try
    {
        // Process the results for both primary and secondary types
        for (const auto& row : results)
        {
            std::uint32_t defendingTypeId = std::stoi(row[0]);
            std::uint32_t attackingTypeId = std::stoi(row[1]);
            double damageMultiplier       = std::stod(row[2]);

            if (effectivenessMap.find(defendingTypeId) != effectivenessMap.end())
            {
                effectivenessMap[defendingTypeId] *= damageMultiplier;
            }
            else
            {
                effectivenessMap[defendingTypeId] = damageMultiplier;
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
        this->logger_.critical("Pokedex::queryTypeEffectivnessTable caught an exception: " +
                               std::string(e.what()));
        return Json::Value(Json::arrayValue);
    }

    return pokemonTypeEffective;
}

Json::Value Pokedex::queryEvolutionTable(const std::uint32_t& targetPokedexNumber)
{
    this->logger_.debug("Pokedex::queryEvolutionTable invoked to query: " +
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
        this->connectEvolutionaryChain(evolutionData);

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
            Json::Value baseButton = this->pokemonButtonDataToJsonValue(baseButtonData);

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
            Json::Value evolvedButton = this->pokemonButtonDataToJsonValue(evolvedButtonData);

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
        this->logger_.critical("Pokedex::queryEvolutionTable caught an exception: " +
                               std::string(e.what()));
        return Json::Value(Json::arrayValue);
    }
}

Json::Value Pokedex::queryLocationTable(const std::uint32_t& pokedexNumber,
                                        const std::string& regionalFormId)
{
    this->logger_.debug("Pokedex::queryLocationTable invoked to query: " +
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
                this->logger_.debug("Pokedex::queryLocationTable found the location of: " +
                                    pokemonLocation);

                pokemonLocations.append(Json::Value(pokemonLocation));
            }
        }
    }
    catch (const std::invalid_argument& e)
    {
        this->logger_.critical("Pokedex::queryLocationTable caught an exception: " +
                               std::string(e.what()));
        return Json::Value(Json::arrayValue);
    }

    return pokemonLocations;
}

Json::Value Pokedex::queryLevelUpMovesetTable(const std::uint32_t& pokedexNumber,
                                              const std::string& regionalFormId)
{
    this->logger_.debug("Pokedex::queryLevelUpMovesetTable invoked to query: " +
                        std::to_string(pokedexNumber) + " and " + regionalFormId);

    Json::Value pokemonLevelUpMoveset(Json::arrayValue);

    this->sqlManager_.prepareStatement(
        "SELECT move_id, mainline_game_id, level_learned FROM Pokemon_Level_Up_Moveset WHERE "
        "pokedex_number = ? AND region_id IS ? ORDER BY level_up_moveset_id;");
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
            this->logger_.warning(
                "No Pokemon LevelUp Moveset was found that matches the Pokedex Number " +
                pokedexNumber);
            return pokemonLevelUpMoveset;
        }

        for (const auto& row : results)
        {
            Json::Value pokemonLevelUpMove;

            pokemonLevelUpMove["move"]          = this->queryMoveTable(std::stoi(row[0]));
            pokemonLevelUpMove["mainline"]      = row[1];
            pokemonLevelUpMove["level_learned"] = row[2];

            pokemonLevelUpMoveset.append(pokemonLevelUpMove);
        }
    }
    catch (const std::invalid_argument& e)
    {
        this->logger_.critical("Pokedex::queryLevelUpMovesetTable caught an exception: " +
                               std::string(e.what()));
        return Json::Value(Json::arrayValue);
    }

    return pokemonLevelUpMoveset;
}

std::pair<Json::Value, Json::Value> Pokedex::queryTechnicalMovesetTable(
    const std::uint32_t& pokedexNumber, const std::string& regionalFormId)
{
    this->logger_.debug("Pokedex::queryTechnicalMovesetTable invoked to query: " +
                        std::to_string(pokedexNumber) + " and " + regionalFormId);

    Json::Value tmMoves(Json::arrayValue);
    Json::Value trMoves(Json::arrayValue);

    this->sqlManager_.prepareStatement(
        "SELECT mainline_game_id, technical_move_id FROM Pokemon_Technical_Moveset WHERE "
        "pokedex_number = ? AND region_id IS ? ORDER BY technical_moveset_id;");
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
            this->logger_.warning(
                "No Pokemon Technical Moveset was found that matches the Pokedex Number " +
                pokedexNumber);
            return {tmMoves, trMoves};
        }

        for (const auto& row : results)
        {
            Json::Value pokemonTechnicalMove;
            auto mainlineId  = row[0];
            auto technicalId = std::stoi(row[1]);

            Json::Value moveData;
            moveData["mainline"]       = mainlineId;
            moveData["technical_move"] = this->queryTechnicalMoveTable(technicalId);

            const std::string isTR = moveData["technical_move"]["is_tr"].asString();
            if (isTR == "0")
            {
                tmMoves.append(moveData);
            }
            else if (isTR == "1")
            {
                trMoves.append(moveData);
            }
            else
            {
                this->logger_.warning("Unrecognized is_tr value: " + isTR);
            }
        }
    }
    catch (const std::invalid_argument& e)
    {
        this->logger_.critical("Pokedex::queryTechnicalMovesetTable caught an exception: " +
                               std::string(e.what()));
    }

    return {tmMoves, trMoves};
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
                pokemon["name"] = Json::Value(this->queryRegionalFormTable(std::stoi(regionId)) +
                                              " " + pokemon["name"].asString());
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
