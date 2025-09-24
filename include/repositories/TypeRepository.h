#ifndef TYPEREPOSITORY_H_
#define TYPEREPOSITORY_H_

#include "Repository.h"

#include <json/json.h>
#include <map>

class TypeRepository : public Respository
{
    public:
        TypeRepository(SQLManager& sqlManager);

        ~TypeRepository() = default;

        std::string queryTypeTable(const std::uint32_t& id);

        Json::Value queryTypeEffectivnessTable(const std::uint32_t& primaryTypeId, const std::uint32_t& secondaryTypeId);

        Json::Value getTypes() const;

        bool typeExists(const std::uint32_t& id) const;

    private:
        void loadTypeTable();
        
        std::map<std::uint32_t, std::string> types_;
};

#endif