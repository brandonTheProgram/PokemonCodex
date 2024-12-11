
#include "Pokedex.h"
#include "Config.h"

Pokedex::Pokedex() : sqlManager(Config::getInstance().get("DATABASE_PATH")), logger(Logger::getInstance()) {}

Json::Value Pokedex::getRegionData(const std::string& region, const bool& shouldLimit) {    
    Json::Value regionData(Json::arrayValue);
    int limit;

    this->logger.debug("Pokedex::getRegionData invoked");

    // Get the region enum based on the value passed in from the front end
    Region regionEnum = this->stringToRegionEnum(region);
    
    if(regionEnum == Region::UNSUPPORTED) {
        return Json::Value{};
    }

    // Get the respective region range
    RegionPair regionPair = this->getRegionPair(regionEnum);

    if(regionPair.first == 0 && regionPair.second) {
        return Json::Value{};
    }

    // Grab the Pokemon from the respective region
    this->logger.info("Pokedex::getRegionData Grabbing the Pokemon from the " + region + " region");
    this->sqlManager.prepareStatement("SELECT pokedex_number, region_id, name, image FROM Pokemon WHERE pokedex_number BETWEEN ? AND ?;");
    this->sqlManager.bind(1, regionPair.first);

    // Grab the start of the region for the homepage
    if(shouldLimit) {
        limit = this->getLimitEnvVar();

        if(((regionPair.second - regionPair.first) - limit) >= 0) {
            this->sqlManager.bind(2, regionPair.first + limit - 1);
        }
        else {
            this->sqlManager.bind(2, regionPair.first + 1);
        }
    }
    else {
        this->sqlManager.bind(2, regionPair.second);
    }

    auto results = this->sqlManager.fetchResults();

    if(shouldLimit && results.size() > limit) {
        results.resize(limit);
    }

    return this->pokemonButtonDataToJsonValue(results);
}

Json::Value Pokedex::getLatestsPokemon() {
    Json::Value latestPokemon(Json::arrayValue);

    this->logger.debug("Pokedex::getLatestsPokemon invoked");

    // Grab the amount of latest Pokemon from the env variable
    int limit = this->getLimitEnvVar(true);

    // Get the latest region range
    RegionPair regionPair = this->getRegionPair(Region::ALL);

    // Grab the latest Pokemon
    if(limit < regionPair.first || limit > regionPair.second) {
        this->logger.warning("The limit of " + std::to_string(limit) + " is out of range. The latests Pokemon will be limited to 1");
        this->sqlManager.prepareStatement("SELECT pokedex_number, region_id, name, image FROM Pokemon ORDER BY pokedex_number DESC LIMIT 1;");
    }
    else {
        this->logger.info("Pokedex::getLatestsPokemon Grabbing the latests " + std::to_string(limit) + " Pokemon");
        this->sqlManager.prepareStatement("SELECT pokedex_number, region_id, name, image FROM Pokemon ORDER BY pokedex_number DESC LIMIT ?;");
        this->sqlManager.bind(1, limit);
    }

    auto results = this->sqlManager.fetchResults();

    return this->pokemonButtonDataToJsonValue(results);
}

Json::Value Pokedex::getRegionNames() const {
    Json::Value regionNames(Json::arrayValue);

    this->logger.debug("Pokedex::getRegionNames invoked");

    for(int i = 1; i < static_cast<int>(Region::ALL); ++i) {
        Json::Value region(Json::objectValue);

        region["name"] = this->regionEnumToString(static_cast<Region>(i));

        regionNames.append(region);
    }

    return regionNames;
}

RegionPair Pokedex::getRegionPair(const Region& regionEnum) const {
    RegionPair regionPair;

    this->logger.debug("Pokedex::getRegionPair invoked");

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
            this->logger.warning("Pokedex::getRegionPair The region enum is not supported: " + static_cast<int>(regionEnum));
    }
    return regionPair;
}

Region Pokedex::stringToRegionEnum(const std::string& region) const {
    this->logger.debug("Pokedex::stringToRegionEnum invoked");

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
        this->logger.warning("Pokedex::stringToRegionEnum The region: " + region + " is currently unsupported");
        return Region::UNSUPPORTED;
    }
}

std::string Pokedex::regionEnumToString(const Region& region) const {
    this->logger.debug("Pokedex::regionEnumToString invoked");

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
        this->logger.warning("Pokedex::regionEnumToString The region enum is not supported: " + static_cast<int>(region));
        return "UNSUPPORTED";
    }
}

Json::Value Pokedex::pokemonButtonDataToJsonValue(const std::vector<std::vector<std::string>>& results) const {
    Json::Value pokemonButtons(Json::arrayValue);

    this->logger.debug("Pokedex::pokemonButtonDataToJsonValue invoked");

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
                    this->logger.warning("Pokedex::pokemonButtonDataToJsonValue The regional form enum is not supported: " + std::stoi(regionId));
                    break;
            }
        }

        logger.debug("Pokedex::pokemonButtonDataToJsonValue Found: " + pokemon["name"].asString());

        pokemonButtons.append(pokemon);
    }

    return pokemonButtons;
}

int Pokedex::getLimitEnvVar(const bool& latest) const {
    int limit = 1;
    std::string envVar;

    try {
        if(latest) {
            envVar = Config::getInstance().get("LATEST_LIMIT");
            this->logger.debug("Found the latest limit environment variable: " + envVar);
        }
        else {
            envVar = Config::getInstance().get("STARTING_LIMIT");
            this->logger.debug("Found the starting limit environment variable: " + envVar);
        }

        limit = std::stoi(envVar);
    }
    catch(const std::exception& e) {
        this->logger.warning("Pokedex::getLimitEnvVar: " + std::string(e.what()));
    }

    return limit;
}