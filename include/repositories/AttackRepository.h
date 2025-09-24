#ifndef ATTACKREPOSITORY_H_
#define ATTACKREPOSITORY_H_

#include "Repository.h"

#include <json/json.h>

class TypeRepository;

class AttackRepository : public Repository
{
    public:
        AttackRepository(SQLManager& sqlManager, TypeRepository& typeRepository);

        ~AttackRepository() = default;

        std::string queryMoveCategoryTable(const uint32_t id);

        Json::Value queryMoveTable(const uint32_t id);

        Json::Value queryTechnicalMoveTable(const uint32_t id);

        Json::Value queryLevelUpMovesetTable(const uint32_t pokedexNumber, const std::string& regionalFormId);

        std::pair<Json::Value, Json::Value> queryTechnicalMovesetTable(const uint32_t pokedexNumber, const std::string& regionalFormId);
    private:
        TypeRepository& typeRepository_;
};

#endif