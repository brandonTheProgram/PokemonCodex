
#include "repositories/TypeRepository.h"
#include "SQLManager.h"

TypeRepository::TypeRepository(SQLManager& sqlManager) : Repository(sqlManager)
{
    this->loadTypeTable();
}

std::string TypeRepository::queryTypeTable(const uint32_t id)
{
    this->logger_.debug("TypeRepository::queryTypeTable invoked to query: " + std::to_string(id));

    if(!this->typeExists(id))
    {
        this->logger_.warning("No type was found that matches the id of " + std::to_string(id));
        return "";
    }

    return this->types_.at(id);
}

Json::Value TypeRepository::queryTypeEffectivenessTable(const uint32_t primaryTypeId, const uint32_t secondaryTypeId)
{
    this->logger_.debug("TypeRepository::queryTypeEffectivenessTable invoked to query: " +
                        std::to_string(primaryTypeId) + " and " + std::to_string(secondaryTypeId));

    Json::Value pokemonTypeEffective(Json::arrayValue);

    auto results = this->sqlManager_.query("SELECT defending_type_id, attacking_type_id, damage_multiplier FROM Pokemon_Type_Effectiveness WHERE attacking_type_id IN (?, ?)  ORDER BY defending_type_id ASC;", [primaryTypeId, secondaryTypeId](SQLite::Statement& statement){
        statement.bind(1, primaryTypeId);
        statement.bind(2, secondaryTypeId);
    });

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
        this->logger_.critical("TypeRepository::queryTypeEffectivenessTable caught an exception: " +
                               std::string(e.what()));
        return Json::Value(Json::arrayValue);
    }

    return pokemonTypeEffective;
}

Json::Value TypeRepository::getTypes() const
{
    Json::Value pokemonTypes(Json::arrayValue);

    this->logger_.debug("TypeRepository::getTypes invoked");

    for (const auto& pair : this->types_)
    {
        Json::Value type(Json::objectValue);
        type["name"] = pair.second;

        pokemonTypes.append(type);
    }

    return pokemonTypes;
}

bool TypeRepository::typeExists(const uint32_t id) const
{
    return this->types_.find(id) != this->types_.end();
}

void TypeRepository::loadTypeTable()
{
    this->logger_.debug("TypeRepository::loadTypeTable invoked");

    auto results = this->sqlManager_.query("SELECT type_id, type_name FROM Pokemon_Type ORDER BY type_id ASC;");

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
        std::uint32_t id = std::stoi(Json::Value(row.at(0)).asString());
        std::string name = Json::Value(row.at(1)).asString();

        this->types_.emplace(id, name);
    }

    this->logger_.debug("TypeRepository::loadTypeTable Found " +
                        std::to_string(this->types_.size()) + " Pokemon Types");
}