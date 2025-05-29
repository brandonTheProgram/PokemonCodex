#!/bin/bash

# Path to the SQLite .db file
DB_FILE="./db/Data/Pokedex.db"

# List of SQL files to execute in order
SQL_FILES=(
    "./db/Schemas/Schema.sql"
    "./db/Schemas/PokemonType.sql"
    "./db/Schemas/PokemonRegion.sql"
    "./db/Schemas/PokemonTypeEffectiveness.sql"
    "./db/Schemas/PokemonMoveCategory.sql"
    "./db/Schemas/PokemonRegionalForms.sql"
    "./db/Schemas/PokemonAbility.sql"
    "./db/Schemas/PokemonGame.sql"
    "./db/Schemas/PokemonMainlineGame.sql"
    "./db/Schemas/PokemonMove.sql"
    "./db/Schemas/Pokemon.sql"
    "./db/Schemas/PokemonEvolution.sql"
    "./db/Schemas/PokemonLocation.sql"
    "./db/Schemas/PokemonTechnicalMove.sql"
    "./db/Schemas/PokemonLevelUpMoveset-Kanto.sql"
    "./db/Schemas/PokemonLevelUpMoveset-Johto.sql"
    "./db/Schemas/PokemonLevelUpMoveset-Hoenn.sql"
    "./db/Schemas/PokemonLevelUpMoveset-Sinnoh.sql"
)

# Remove the existing database file if it exists
if [ -f "$DB_FILE" ]; then
    echo "Removing existing database: $DB_FILE"
    rm "$DB_FILE"
fi

# Create a fresh new database file
echo "Creating new database: $DB_FILE"

# Loop through each SQL file
for sql_file in "${SQL_FILES[@]}"; do
    echo "Applying $sql_file..."
    sqlite3 "$DB_FILE" < "$sql_file"
    if [ $? -eq 0 ]; then
        echo "$sql_file applied successfully."
    else
        echo "Error applying $sql_file."
        exit 1
    fi
done

echo "Database reset and all SQL files applied."