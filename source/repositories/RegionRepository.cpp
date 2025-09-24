#include "repositories/RegionRepository.h"
#include "SQLManager.h"
#include <algorithm>

RegionRepository::RegionRepository(SQLManager& sqlManager) : Respository(sqlManager)
{
    this->loadRegionTable();
}

std::string RegionRepository::queryRegionalFormTable(const std::uint32_t& id)
{
    this->logger_.debug("RegionRepository::queryRegionalFormTable invoked to query: " + std::to_string(id));

    std::string regionalFormName = "";

    this->sqlManager_.prepareStatement(
        "SELECT region_name FROM Pokemon_Regional_Form WHERE region_id = ?;");
    this->sqlManager_.bind(1, id);
    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        this->logger_.warning("No regional form was found that matches the id of " +
                              std::to_string(id));
        return regionalFormName;
    }

    return results[0].at(0);
}

Json::Value RegionRepository::getRegions() const
{
    Json::Value regions(Json::arrayValue);

    this->logger_.debug("RegionRepository::getRegions invoked");

    for(const auto& pair : this->regions_)
    {
        Json::Value region(Json::objectValue);
        region["name"]  = pair.second.name;
        region["image"] = pair.second.image;
        region["start"] = pair.second.start;
        region["end"] = pair.second.end;

        regions.append(region);
    }

    return regions;
}

std::pair<std::uint32_t, std::uint32_t> RegionRepository::getEndpoints(const std::string& region) const
{
    if(!this->regionExists(region))
    {
        this->logger_.warning("The following region is not valid: " + region);
        return {};
    }
    auto it = std::find_if(this->regions_.begin(), this->regions_.end(),
                           [&region](const auto& pair) {
                               return pair.second.name == region;
                           });
    Region foundRegion = it->second;

    return {foundRegion.start, foundRegion.end};
}

 bool RegionRepository::regionExists(const std::string& region) const
 {
    auto it = std::find_if(this->regions_.begin(), this->regions_.end(),
                           [&region](const auto& pair) {
                               return pair.second.name == region;
                           });
    return it != this->regions_.end();
 }

void RegionRepository::loadRegionTable()
{
    this->logger_.debug("RegionRepository::loadRegionTable invoked");

    this->sqlManager_.prepareStatement("SELECT region_id, name, image, start, end FROM Pokemon_Region ORDER BY region_id ASC;");
    auto results = this->sqlManager_.fetchResults();

    if (results.empty())
    {
        std::string message =
            "No regions were found in the database, verify that Pokemon_Region table is populated "
            "in the database";
        this->logger_.critical(message);
        throw std::runtime_error(message);
    }

    try
    {
        for (const auto& row : results)
        {
            std::uint32_t id = std::stoi(Json::Value(row.at(0)).asString());
            std::string name    = Json::Value(row.at(1)).asString();
            std::string image   = Json::Value(row.at(2)).asString();
            std::uint32_t start = std::stoi(Json::Value(row.at(3)).asString());
            std::uint32_t end   = std::stoi(Json::Value(row.at(4)).asString());

            this->regions_[id] = Region(name, image, start, end);
        }
    }
    catch (const std::invalid_argument& e)
    {
        this->logger_.critical("RegionRepository::loadRegionTable caught an exception: " +
                               std::string(e.what()));
        throw e;
    }

    this->logger_.debug("RegionRepository::loadRegionTable Found " + std::to_string(this->regions_.size()) +
                        " Pokemon Regions");
}