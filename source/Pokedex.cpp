
#include "Pokedex.h"

Pokedex::Pokedex() : sqlManager(DATABASE_PATH) {}

Json::Value Pokedex::getAllPokedexImages() {
    Json::Value pokedexImages(Json::arrayValue);

    this->sqlManager.prepareStatement("SELECT image FROM Pokemon;");

    auto results = this->sqlManager.fetchResults();

    for (const auto& row : results) {
        for (const auto& col : row) {
            pokedexImages.append(col);
        }
    }

    return pokedexImages;
}