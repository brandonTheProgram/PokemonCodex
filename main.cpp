#include <iostream>
#include "SQLManager.h"

int main()
{
    SQLManager sqlManager(DATABASE_PATH);

    sqlManager.prepareStatement("SELECT * FROM Pokemon;");
    auto results = sqlManager.fetchResults();

    for (const auto& row : results) {
        for (const auto& col : row) {
            std::cout << col << " ";
        }
        std::cout << "\n";
    }

    return 0;
}