#ifndef ATTACKREPOSITORY_H_
#define ATTACKREPOSITORY_H_

#include "Repository.h"

#include <json/json.h>

class TypeRepository;

class AttackRepository : public Respository
{
    public:
        AttackRepository(SQLManager& sqlManager, TypeRepository& typeRepository);

        ~AttackRepository() = default;

        std::string queryMoveCategoryTable(const std::uint32_t& id);

        Json::Value queryMoveTable(const std::uint32_t& id);

        Json::Value queryTechnicalMoveTable(const std::uint32_t& id);

        Json::Value queryLevelUpMovesetTable(const std::uint32_t& pokedexNumber, const std::string& regionalFormId);

        std::pair<Json::Value, Json::Value> queryTechnicalMovesetTable(const std::uint32_t& pokedexNumber, const std::string& regionalFormId);
    private:
        TypeRepository& typeRepository_;
};

#endif