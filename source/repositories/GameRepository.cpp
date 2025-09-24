
#include "repositories/GameRepository.h"
#include "SQLManager.h"

GameRepository::GameRepository(SQLManager& sqlManager) : Repository(sqlManager)
{
    this->loadGameTable();
    this->loadMainlineGameTable();
}

Json::Value GameRepository::getGames() const
{
    Json::Value games(Json::arrayValue);

    this->logger_.debug("GameRepository::getGames invoked");

    for (const auto& pair : this->games_)
    {
        Json::Value type(Json::objectValue);
        type["name"] = pair.second;

        games.append(type);
    }

    return games;
}

Json::Value GameRepository::getMainlineGames() const
{
    Json::Value mainlineGames(Json::arrayValue);

    this->logger_.debug("GameRepository::getMainlineGames invoked");

    for (const auto& pair : this->mainlineGames_)
    {
        Json::Value game(Json::objectValue);
        game["id"]   = pair.first;
        game["name"] = pair.second;

        mainlineGames.append(game);
    }

    return mainlineGames;
}

void GameRepository::loadGameTable()
{
    this->logger_.debug("GameRepository::loadGameTable invoked");

    auto results = this->sqlManager_.query("SELECT game_id, game_name FROM Pokemon_Game ORDER BY game_id ASC;");

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
        std::uint32_t id = std::stoi(Json::Value(row.at(0)).asString());
        std::string name = Json::Value(row.at(1)).asString();

        this->games_.emplace(id, name);
    }

    this->logger_.debug("GameRepository::loadGameTable Found " +
                        std::to_string(this->games_.size()) + " Pokemon Games");
}

void GameRepository::loadMainlineGameTable()
{
    this->logger_.debug("GameRepository::loadMainlineGameTable invoked");

    auto results = this->sqlManager_.query("SELECT mainline_game_id, game_name FROM Pokemon_Mainline_Game ORDER BY mainline_game_id ASC;");

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
        std::uint32_t id = std::stoi(Json::Value(row.at(0)).asString());
        std::string name = Json::Value(row.at(1)).asString();
        this->mainlineGames_.emplace(id, name);
    }

    this->logger_.debug("GameRepository::loadMainlineGameTable Found " +
                        std::to_string(this->mainlineGames_.size()) +
                        " Mainline Pokemon Games");
}