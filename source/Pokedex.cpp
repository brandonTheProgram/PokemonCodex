
#include "Pokedex.h"

Pokedex::Pokedex() : sqlManager(DATABASE_PATH), logger(Logger::getInstance()) {}

Json::Value Pokedex::getRegionData(const std::string& region) {    
    Json::Value regionData(Json::arrayValue);

    // Get the region enum based on the value passed in from the front end
    Region regionEnum = this->stringToRegionEnum(region);
    
    if(regionEnum == Region::UNSUPPORTED) {
        return regionData;
    }

    // Get the respective region range
    RegionPair regionPair = this->getRegionPair(regionEnum);

    if(regionPair.first == 0 && regionPair.second) {
        return regionData;
    }

    // Grab the Pokemon from the respective region
    logger.info("Pokedex::getRegionData Grabbing the Pokemon from the " + region + " region");
    this->sqlManager.prepareStatement("SELECT pokedex_number, region_id, name, image FROM Pokemon WHERE pokedex_number BETWEEN ? AND ?;");
    this->sqlManager.bind(1, regionPair.first);
    this->sqlManager.bind(2, regionPair.second);

    auto results = this->sqlManager.fetchResults();

    for (const auto& row : results) {
        Json::Value pokemon(Json::objectValue);

        pokemon["pokedex_number"] = row.at(0);
        pokemon["region_id"] = row.at(1);
        pokemon["name"] = row.at(2);
        pokemon["image"] = row.at(3);

        // Apply the region form to the name if present
        std::string regionId = pokemon["region_id"].asString();
        if(regionId != "NULL") {
            // Append the name of the regional form to the Pokemon's name
            switch(static_cast<RegionalForm>(std::stoi(regionId))) {
                case RegionalForm::ALOLA:
                    pokemon["name"] = Json::Value("Alolan " + pokemon["name"].asString());
                    break;
                case RegionalForm::GALAR:
                    pokemon["name"] = Json::Value("Galarian " + pokemon["name"].asString());
                    break;
                case RegionalForm::HISIUI:
                    pokemon["name"] = Json::Value("Hisuian " + pokemon["name"].asString());
                    break;
                case RegionalForm::PALDEA:
                    pokemon["name"] = Json::Value("Paldean " + pokemon["name"].asString());
                    break;
                default:
                    break;
            }
        }

        logger.info("Pokedex::getRegionData Found: " + pokemon["name"].asString());

        regionData.append(pokemon);
    }

    return regionData;
}

Json::Value Pokedex::getRegionNames() const {
    Json::Value regionNames(Json::arrayValue);

    for(int i = 1; i < static_cast<int>(Region::ALL); ++i) {
        Json::Value region(Json::objectValue);

        region["name"] = this->regionEnumToString(static_cast<Region>(i));

        regionNames.append(region);
    }

    return regionNames;
}

RegionPair Pokedex::getRegionPair(const Region& regionEnum) const {
    RegionPair regionPair;

    switch(regionEnum) {
        case Region::KANTO:
            regionPair = KANTO_PAIR;
            break;
        case Region::JOHTO:
            regionPair = JOHTO_PAIR;
            break;
        case Region::HOENN:
            regionPair = HOENN_PAIR;
            break;
        case Region::SINNOH:
            regionPair = SINNOH_PAIR;
            break;
        case Region::UNOVA:
            regionPair = UNOVA_PAIR;
            break;
        case Region::KALOS:
            regionPair = KALOS_PAIR;
            break;
        case Region::ALOLA:
            regionPair = ALOLA_PAIR;
            break;
        case Region::GALAR:
            regionPair = GALAR_PAIR;
            break;
        case Region::PALDEA:
            regionPair = PALDEA_PAIR;
            break;
        case Region::ALL:
            regionPair = ALL_PAIR;
            break;
        default:
            logger.warning("Pokedex::getRegionPair The region enum is not supported: " + static_cast<int>(regionEnum));
    }
    return regionPair;
}

Region Pokedex::stringToRegionEnum(const std::string& region) const {
    if(region == "Kanto") {
        return Region::KANTO;
    }
    else if(region == "Johto") {
        return Region::JOHTO;
    }
    else if(region == "Hoenn") {
        return Region::HOENN;
    }
    else if(region == "Sinnoh") {
        return Region::SINNOH;
    }
    else if(region == "Unova") {
        return Region::UNOVA;
    }
    else if(region == "Kalos") {
        return Region::KALOS;
    }
    else if(region == "Alola") {
        return Region::ALOLA;
    }
    else if(region == "Galar") {
        return Region::GALAR;
    }
    else if(region == "Paldea") {
        return Region::PALDEA;
    }
    else if(region == "All") {
        return Region::ALL;
    }
    else {
        logger.warning("Pokedex::stringToRegionEnum The region: " + region + " is currently unsupported");
        return Region::UNSUPPORTED;
    }
}

std::string Pokedex::regionEnumToString(const Region& region) const {
    if(region == Region::KANTO) {
        return "Kanto";
    }
    else if(region == Region::JOHTO) {
        return "Johto";
    }
    else if(region == Region::HOENN) {
        return "Hoenn";
    }
    else if(region == Region::SINNOH) {
        return "Sinnoh";
    }
    else if(region == Region::UNOVA) {
        return "Unova";
    }
    else if(region == Region::KALOS) {
        return "Kalos";
    }
    else if(region == Region::ALOLA) {
        return "Alola";
    }
    else if(region == Region::GALAR) {
        return "Galar";
    }
    else if(region == Region::PALDEA) {
        return "Paldea";
    }
    else if(region == Region::ALL) {
        return "All";
    }
    else {
        logger.warning("Pokedex::regionEnumToString The region enum is not supported: " + static_cast<int>(region));
        return "UNSUPPORTED";
    }
}