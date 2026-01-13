#ifndef TYPEREPOSITORY_H_
#define TYPEREPOSITORY_H_

#include "Repository.h"

#include <json/json.h>
#include <map>

/**
 * @class TypeRepository
 * @brief An extension of the Repository class for Pokemon Types.
 * 
 * This class manages the database for anything related to Pokemon Attacks:
 *    Pokemon Type
 *    Pokemon Type Effectiveness Table
 */
class TypeRepository : public Repository
{
    public:
        /**
         * @brief Constructs a new TypeRepository object.
         * @param sqlManager The sql manager instance that allows queries.
         */
        TypeRepository(SQLManager& sqlManager);

        /**
         * @brief Destroys the AttackRepository object.
         */
        ~TypeRepository() = default;

        /**
         * @brief Query the Move Category Table.
         * @param id The id of the move to query.
         * @return A string of the results.
         */
        std::string queryTypeTable(const uint32_t id);

        /**
         * @brief Query the Type Table.
         * @param primaryTypeId The primary id of a Pokemon Move.
         * @param secondaryTypeId The secondary id of a Pokemon Move.
         * @return A Json value object of the results.
         */
        Json::Value queryTypeEffectivenessTable(const uint32_t primaryTypeId, const uint32_t secondaryTypeId);

        /**
         * @brief Iterate over the map for Pokemon Types.
         * @return A Json value object of the results.
         */
        Json::Value getTypes() const;

        /**
         * @brief Search if the type exists.
         * @param id The target type.
         * @return A flag if the region exists.
         */
        bool typeExists(const uint32_t id) const;

    private:
        
        /**
         * @brief Query the Pokemon Type Table and store the results.
         */
        void loadTypeTable();
        
        std::map<std::uint32_t, std::string> types_;
};

#endif