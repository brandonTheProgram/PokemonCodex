#ifndef POKEDEX_H_
#define POKEDEX_H_

#include "SQLManager.h"
#include <utility>
#include <json/json.h>

enum class RegionalForm
{
    ALOLA = 1,
    GALAR = 2,
    HISIUI = 3,
    PALDEA = 4
};

enum class Region
{
    UNSUPPORTED = 0,
    KANTO = 1,
    JOHTO = 2,
    HOENN = 3,
    SINNOH = 4,
    UNOVA = 5,
    KALOS = 6,
    ALOLA = 7,
    GALAR = 8,
    PALDEA = 9,
    ALL = 10
};

typedef std::pair<int, int> RegionPair;

const RegionPair KANTO_PAIR{1, 151};
const RegionPair JOHTO_PAIR{152, 251};
const RegionPair HOENN_PAIR{252, 386};
const RegionPair SINNOH_PAIR{387, 493};
const RegionPair UNOVA_PAIR{494, 649};
const RegionPair KALOS_PAIR{650, 721};
const RegionPair ALOLA_PAIR{722, 809};
const RegionPair GALAR_PAIR{810, 905};
const RegionPair PALDEA_PAIR{906, 1025};
const RegionPair ALL_PAIR{1, 1025};

class Pokedex
{
public:
    Pokedex();
    ~Pokedex() = default;

    Json::Value getRegionData(const std::string& region);

private:
    RegionPair getRegionPair(const Region& regionEnum) const;
    Region stringToRegionEnum(const std::string& region) const;
    std::string regionEnumToString(const Region& region) const;
    RegionalForm stringToRegionalFormEnum(const std::string& regionalForm) const;
    std::string regionalFormEnumToString(const RegionalForm& regionalForm) const;

    SQLManager sqlManager;
    Logger& logger;
};

#endif