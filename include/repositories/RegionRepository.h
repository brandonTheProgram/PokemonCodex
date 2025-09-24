#ifndef REGIONREPOSITORY_H_
#define REGIONREPOSITORY_H_

#include "Repository.h"

#include <json/json.h>
#include <map>
#include <utility>

class RegionRepository : public Respository
{
    struct Region
    {
        std::string name;
        std::string image;
        std::uint32_t start;
        std::uint32_t end;

        Region() = default;
        Region(const std::string& name, const std::string& image, const std::uint32_t& start,
                const std::uint32_t& end)
            : name(name), image(image), start(start), end(end)
        {}

        bool operator==(const Region& other) const 
        {
            return this->name == other.name && this->image == other.image && this->start == other.start && this->end == other.end;
        }
    };

    public:
        RegionRepository(SQLManager& sqlManager);

        ~RegionRepository() = default;

        std::string queryRegionalFormTable(const std::uint32_t& id);

        Json::Value getRegions() const;

        std::pair<std::uint32_t, std::uint32_t> getEndpoints(const std::string& region) const;

        bool regionExists(const std::string& region) const;

    private:
        void loadRegionTable();

        std::map<std::uint32_t, Region> regions_;
};

#endif