#ifndef GAMEREPOSITORY_H_
#define GAMEREPOSITORY_H_

#include "Repository.h"

#include <json/json.h>
#include <map>

/**
 * @class GameRepository
 * @brief An extension of the Repository class for Pokemon Games.
 * 
 * This class manages the database for anything related to Pokemon Attacks:
 *    Pokemon Games (Sword, Shield, Scarlet, Violet, etc)
 *    Pokemon Mainelines Games (Sword & Shield, Legends: Arceus, etc)
 */
class GameRepository : public Repository
{
    public:
        /**
         * @brief Constructs a new GameRepository object.
         * @param sqlManager The sql manager instance that allows queries.
         */
        GameRepository(SQLManager& sqlManager);

        /**
         * @brief Destroys the GameRepository object.
         */
        ~GameRepository() = default;

        /**
         * @brief Iterate over the map and return the games.
         * @return A Json value object of the results.
         */
        Json::Value getGames() const;

        /**
         * @brief Iterate over the map and return the mainline games.
         * @return A Json value object of the results.
         */
        Json::Value getMainlineGames() const;

    private:

        /**
         * @brief Query the Pokemon Games Table and store the results.
         */
        void loadGameTable();

        /**
         * @brief Query the Pokemon Maineline Games Table and store the results.
         */
        void loadMainlineGameTable();

        std::map<std::uint32_t, std::string> games_;
        std::map<std::uint32_t, std::string> mainlineGames_;
};

#endif