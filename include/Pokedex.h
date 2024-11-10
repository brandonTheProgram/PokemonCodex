#ifndef POKEDEX_H_
#define POKEDEX_H_

#include "SQLManager.h"
#include <json/json.h>

class Pokedex
{
public:
    Pokedex();
    ~Pokedex() = default;

    Json::Value getAllPokedexImages();

private:
    SQLManager sqlManager;
};

#endif