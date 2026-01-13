#ifndef REGIONREPOSITORY_H_
#define REGIONREPOSITORY_H_

#include "Repository.h"

#include <json/json.h>
#include <map>
#include <utility>

/**
 * @class RegionRepository
 * @brief An extension of the Repository class for Regions.
 * 
 * This class manages the database for anything related to a Pokemon Region:
 *    Region
 *    Starting and Ending points of a region
 */
class RegionRepository : public Repository
{
    /**
     * @struct Region
     * @brief Represents an individual Pokemon Region.
     */
    struct Region
    {
        std::string name;    ///< The Name of the region.
        std::string image;   ///< The Image of the region.
        std::uint32_t start; ///< The starting index of the region.
        std::uint32_t end;   ///< The ending index of the region.

        /**
         * @brief Constructs a new Region object.
         */
        Region() = default;

        /**
         * @brief Constructs a new Region object.
         * @param name The Name of the region.
         * @param image The Image of the region.
         * @param start The starting index of the region.
         * @param end The ending index of the region.
         */
        Region(const std::string& name, const std::string& image, const uint32_t start,
                const uint32_t end)
            : name(name), image(image), start(start), end(end)
        {}

        /**
         * @brief Checks if two Region objects are equal.
         * 
         * @param other The other Region to compare against.
         * @return true if the objects match, false otherwise.
         */
        bool operator==(const Region& other) const 
        {
            return this->name == other.name && this->image == other.image && this->start == other.start && this->end == other.end;
        }
    };

    public:

        /**
         * @brief Constructs a new RegionRepository object.
         * @param sqlManager The sql manager instance that allows queries.
         */
        RegionRepository(SQLManager& sqlManager);

        /**
         * @brief Destroys the RegionRepository object.
         */
        ~RegionRepository() = default;

        /**
         * @brief Query for a regional form.
         * @param id The target regional form.
         * @return A string result.
         */
        std::string queryRegionalFormTable(const uint32_t id);

        /**
         * @brief Iterate over the map for Pokemon Regions.
         * @return A Json value object of the results.
         */
        Json::Value getRegions() const;

        /**
         * @brief Get the endpoints of a Pokemon Region.
         * @param region The target region.
         * @return A pair of endpoints for the target region.
         */
        std::pair<std::uint32_t, std::uint32_t> getEndpoints(const std::string& region) const;

        /**
         * @brief Search if the region exists.
         * @param region The target region.
         * @return A flag if the region exists.
         */
        bool regionExists(const std::string& region) const;

    private:
        /**
         * @brief Query the Pokemon Region table and store the results.
         */
        void loadRegionTable();

        std::map<std::uint32_t, Region> regions_;
};

#endif