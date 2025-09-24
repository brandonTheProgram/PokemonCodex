
#include "repositories/AttackRepository.h"
#include "repositories/TypeRepository.h"
#include "SQLManager.h"

AttackRepository::AttackRepository(SQLManager& sqlManager, TypeRepository& typeRepository) : Respository(sqlManager), typeRepository_(typeRepository) {}

std::string AttackRepository::queryMoveCategoryTable(const std::uint32_t& id)
{
    this->logger_.debug("AttackRepository::queryMoveCategoryTable invoked to query: " + std::to_string(id));

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

Json::Value AttackRepository::queryMoveTable(const std::uint32_t& id)
{
    this->logger_.debug("AttackRepository::queryMoveTable invoked to query: " + std::to_string(id));

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
    pokemonMove["type"]        = this->typeRepository_.queryTypeTable(std::stoi(results[0].at(2)));
    pokemonMove["category"]    = this->queryMoveCategoryTable(std::stoi(results[0].at(3)));
    pokemonMove["power"]       = results[0].at(4);
    pokemonMove["accuracy"]    = results[0].at(5);
    pokemonMove["pp"]          = results[0].at(6);

    return pokemonMove;
}

Json::Value AttackRepository::queryTechnicalMoveTable(const std::uint32_t& id)
{
    this->logger_.debug("AttackRepository::queryTechnicalMoveTable invoked to query: " + std::to_string(id));

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

Json::Value AttackRepository::queryLevelUpMovesetTable(const std::uint32_t& pokedexNumber, const std::string& regionalFormId)
{
    this->logger_.debug("AttackRepository::queryLevelUpMovesetTable invoked to query: " +
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
        this->logger_.critical("AttackRepository::queryLevelUpMovesetTable caught an exception: " +
                               std::string(e.what()));
        return Json::Value(Json::arrayValue);
    }

    return pokemonLevelUpMoveset;
}

std::pair<Json::Value, Json::Value> AttackRepository::queryTechnicalMovesetTable(const std::uint32_t& pokedexNumber, const std::string& regionalFormId)
{
    this->logger_.debug("AttackRepository::queryTechnicalMovesetTable invoked to query: " +
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
        this->logger_.critical("AttackRepository::queryTechnicalMovesetTable caught an exception: " +
                               std::string(e.what()));
    }

    return {tmMoves, trMoves};
}