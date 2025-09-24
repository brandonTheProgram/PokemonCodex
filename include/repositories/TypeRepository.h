#ifndef TYPEREPOSITORY_H_
#define TYPEREPOSITORY_H_

#include "Repository.h"

#include <json/json.h>
#include <map>

class TypeRepository : public Repository
{
    public:
        TypeRepository(SQLManager& sqlManager);

        ~TypeRepository() = default;

        std::string queryTypeTable(const uint32_t id);

        Json::Value queryTypeEffectivenessTable(const uint32_t primaryTypeId, const uint32_t secondaryTypeId);

        Json::Value getTypes() const;

        bool typeExists(const uint32_t id) const;

    private:
        void loadTypeTable();
        
        std::map<std::uint32_t, std::string> types_;
};

#endif