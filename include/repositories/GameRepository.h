#ifndef GAMEREPOSITORY_H_
#define GAMEREPOSITORY_H_

#include "Repository.h"

#include <json/json.h>
#include <map>

class GameRepository : public Repository
{
    public:
        GameRepository(SQLManager& sqlManager);

        ~GameRepository() = default;

        Json::Value getGames() const;
        Json::Value getMainlineGames() const;

    private:
        void loadGameTable();
        void loadMainlineGameTable();

        std::map<std::uint32_t, std::string> games_;
        std::map<std::uint32_t, std::string> mainlineGames_;
};

#endif