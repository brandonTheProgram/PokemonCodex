#ifndef ATTACKREPOSITORY_H_
#define ATTACKREPOSITORY_H_

#include "Repository.h"

#include <json/json.h>

class TypeRepository;

/**
 * @class AttackRepository
 * @brief An extension of the Repository class for Pokemon Attacks.
 * 
 * This class manages the database for anything related to Pokemon Attacks:
 *    Pokemon Move
 *    Pokemon Move Category
 *    Technical Moves
 */
class AttackRepository : public Repository
{
    public:
        /**
         * @brief Constructs a new AttackRepository object.
         * @param sqlManager The sql manager instance that allows queries.
         * @param typeRepository The instance of the type repository that allows queries based on type.
         */
        AttackRepository(SQLManager& sqlManager, TypeRepository& typeRepository);

        /**
         * @brief Destroys the AttackRepository object.
         */
        ~AttackRepository() = default;

        /**
         * @brief Query the Move Category Table.
         * @param id The id of the move to query.
         * @return A string of the results.
         */
        std::string queryMoveCategoryTable(const uint32_t id);

        /**
         * @brief Query the Move Table.
         * @param id The id of the move to query.
         * @return A Json value object of the results.
         */
        Json::Value queryMoveTable(const uint32_t id);

        /**
         * @brief Query the Technical Move Table.
         * @param id The id of the move to query.
         * @return A Json value object of the results.
         */
        Json::Value queryTechnicalMoveTable(const uint32_t id);

        /**
         * @brief Query the Levelup Moveset Table.
         * @param pokedexNumber The target of Pokemon to query.
         * @param regionalFormId The regional form of the Pokemon to query.
         * @return A Json value object of the results.
         */
        Json::Value queryLevelUpMovesetTable(const uint32_t pokedexNumber, const std::string& regionalFormId);

        /**
         * @brief Query the Technical Moveset Table.
         * @param pokedexNumber The target of Pokemon to query.
         * @param regionalFormId The regional form of the Pokemon to query.
         * @return A pair of Json value objects of the results.
         */
        std::pair<Json::Value, Json::Value> queryTechnicalMovesetTable(const uint32_t pokedexNumber, const std::string& regionalFormId);
        
    private:
        TypeRepository& typeRepository_;
};

#endif