BEGIN TRANSACTION;

/*******
Pokémon Type Table

Store the unique Pokémon type identifies.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Type (
    type_id INTEGER PRIMARY KEY AUTOINCREMENT,
    type_name TEXT NOT NULL UNIQUE -- e.g., 'Electric', 'Water', 'Fire'
);

/*******
Move Category Table

Store the unique Pokémon move category identifies.
*******/
CREATE TABLE IF NOT EXISTS Move_Category (
    category_id INTEGER PRIMARY KEY AUTOINCREMENT,
    category_name TEXT NOT NULL UNIQUE -- e.g., 'Physical', 'Special', 'Status'
);

/*******
Regional Forms Table

Store the unique region/form identifies.
*******/
CREATE TABLE IF NOT EXISTS Regional_Form (
    region_id INTEGER PRIMARY KEY AUTOINCREMENT,
    region_name TEXT NOT NULL UNIQUE -- e.g., 'Alola', 'Galar', 'Hisui', 'Paldea'
);

/*******
Ability Table

Store the abilitity a Pokémon can have.
*******/
CREATE TABLE IF NOT EXISTS Ability (
    ability_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    description TEXT NOT NULL
);

/*******
Game Table

Store the unique available game identifies.
*******/
CREATE TABLE IF NOT EXISTS Game (
    game_id INTEGER PRIMARY KEY AUTOINCREMENT, -- Unique identifier for each game
    game_name TEXT NOT NULL UNIQUE             -- The name of the game (e.g., Red, Blue, Sword, Shield)
);

/*******
Move Table

Store a move a Pokémon can learn.
*******/
CREATE TABLE IF NOT EXISTS Move (
    move_id INTEGER PRIMARY KEY AUTOINCREMENT,  -- Unique identifier for the move
    move_name TEXT NOT NULL UNIQUE,             -- Name of the move (e.g., "Thunderbolt")
    description TEXT NOT NULL,                  -- Description of the move
    type_id INTEGER NOT NULL,                   -- Reference to the move's type (from Pokemon_Type table)
    category_id INTEGER,                        -- Reference to the move's category (from Move_Category table)
    power INTEGER,                              -- Base power of the move (e.g., 90 for Thunderbolt)
    accuracy FLOAT(5, 2),                       -- Accuracy percentage (e.g., 100.00 for Thunderbolt)
    pp INTEGER,                                 -- Power Points for the move

    -- Key Constraints
    FOREIGN KEY (type_id) REFERENCES Pokemon_Type(type_id),
    FOREIGN KEY (category_id) REFERENCES Move_Category(category_id)
);

/*******
Move Learn Method Table

Store how a Pokémon can learn the move.
*******/
CREATE TABLE IF NOT EXISTS Move_Learn_Method (
    method_id INTEGER PRIMARY KEY AUTOINCREMENT,    -- Unique identifier for the learn method
    method_name TEXT NOT NULL UNIQUE                -- Name of the method (e.g., "Level Up", "TM", "HM", "TR", "Eggmove", "Move Tutor")
);

/*******
Pokémon Table

Stores basic information about each Pokémon.
A junction table to link Pokémon with their regional forms.
*******/
CREATE TABLE IF NOT EXISTS Pokemon (
    pokedex_number INTEGER NOT NULL,        -- National Pokédex number
    region_id INTEGER,                      -- References the region in the Regional_Form table. NULL for a Pokémon not being a regional variant
    name TEXT NOT NULL,                     -- Pokémon's name
    image TEXT NOT NULL,                    -- Pokémon's image
    species TEXT NOT NULL,                  -- Pokémon's species
    primary_type_id INTEGER NOT NULL,       -- Reference to the Pokémon's primary type (from Move_Type table)
    secondary_type_id INTEGER,              -- Reference to the Pokémon's secondary type, if any
    primary_ability_id INTEGER NOT NULL,    -- Reference to the Pokémon's primary ability (from Ability table)
    secondary_ability_id INTEGER,           -- Reference to the Pokémon's secondary ability, if any
    hidden_ability_id INTEGER,              -- Reference to the Pokémon's hidden ability, if any
    description TEXT NOT NULL,              -- Pokédex description

    -- Key Constraints
    PRIMARY KEY (pokedex_number, region_id),
    FOREIGN KEY (primary_type_id) REFERENCES Pokemon_Type(type_id),
    FOREIGN KEY (secondary_type_id) REFERENCES Pokemon_Type(type_id),
    FOREIGN KEY (primary_ability_id) REFERENCES Ability(ability_id),
    FOREIGN KEY (secondary_ability_id) REFERENCES Ability(ability_id),
    FOREIGN KEY (hidden_ability_id) REFERENCES Ability(ability_id),
    FOREIGN KEY (region_id) REFERENCES Regional_Form(region_id)
);

/*******
Evolution Table

Stores a Pokémon's evolution information.
*******/
CREATE TABLE IF NOT EXISTS Evolution (
    evolution_id INTEGER PRIMARY KEY AUTOINCREMENT,
    base_pokedex_number INTEGER NOT NULL, -- The Pokémon before evolution
    base_region_id INTEGER, -- The region/form of the base Pokémon
    evolved_pokedex_number INTEGER NOT NULL, -- The Pokémon after evolution
    evolved_region_id INTEGER, -- The region/form of the evolved Pokémon
    evolution_method TEXT, -- Method of evolution (e.g., level-up, item, trade, etc.)

    -- Key Constraints
    FOREIGN KEY (base_pokedex_number, base_region_id) REFERENCES Pokemon(pokedex_number, region_id),
    FOREIGN KEY (evolved_pokedex_number, evolved_region_id) REFERENCES Pokemon(pokedex_number, region_id)
);

/*******
Pokémon Location Table

Stores the location where the Pokemoin is found in each game.
A junction table to link Pokémon with their game locations.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Location (
    location_id INTEGER PRIMARY KEY AUTOINCREMENT,     -- Unique identifier for each location entry
    pokedex_number INTEGER NOT NULL,               -- Reference to the Pokémon's Pokédex number
    region_id INTEGER,                             -- Reference to the Pokémon's regional form
    game_id INTEGER NOT NULL,                      -- Reference to the game (Red, Blue, Sword, Shield, etc.)
    location_name TEXT NOT NULL,               -- The name of the location(s) (Route 1, Viridian Forest, etc.)
    
    -- Key Constraints
    FOREIGN KEY (pokedex_number, region_id) REFERENCES Pokemon(pokedex_number, region_id),
    FOREIGN KEY (game_id) REFERENCES Game(game_id)
);

/*******
Pokémon Moveset Table

Stores the moveset of the Pokémon for each game.
A junction table to link Pokémon with their game locations.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Moveset (
    pokedex_number INTEGER NOT NULL,           -- Reference to the Pokémon's Pokédex number
    region_id INTEGER,                         -- Reference to the Pokémon's region (for regional forms)
    move_id INTEGER NOT NULL,                  -- Reference to the move
    method_id INTEGER NOT NULL,                -- Reference to how the move is learned
    level_learned INTEGER,                     -- Level at which the move is learned (relevant for Level Up method)
    technical_number INTEGER,                  -- TM/TR number (relevant for TM, TR, etc)
    game_id INTEGER NOT NULL,                  -- Reference to the specific game (Red, Blue, Sword, etc.)

    -- Key Constraints
    PRIMARY KEY (pokedex_number, region_id, move_id, method_id, game_id),
    FOREIGN KEY (pokedex_number, region_id) REFERENCES Pokemon(pokedex_number, region_id),
    FOREIGN KEY (move_id) REFERENCES Move(move_id),
    FOREIGN KEY (method_id) REFERENCES Move_Learn_Method(method_id),
    FOREIGN KEY (game_id) REFERENCES Game(game_id)
);

/*******
Indexing
*******/
-- Index on single columns
CREATE INDEX idx_pokemon_pokedex_number ON Pokemon(pokedex_number);
CREATE INDEX idx_move_move_id ON Move(move_id);
CREATE INDEX idx_pokemon_location_game_id ON Pokemon_Location(game_id);

-- Compound indexes
CREATE INDEX idx_pokemon_moveset_pokedex_region ON Pokemon_Moveset(pokedex_number, region_id);
CREATE INDEX idx_pokemon_moveset_move_id ON Pokemon_Moveset(move_id);
CREATE INDEX idx_pokemon_moveset_method_id ON Pokemon_Moveset(method_id);

-- Partial index for technical_number when not NULL
CREATE INDEX idx_pokemon_moveset_technical_number ON Pokemon_Moveset(technical_number) WHERE technical_number IS NOT NULL;

-- Unique index for pokedex_number and region_id combination
CREATE UNIQUE INDEX idx_pokemon_pokedex_region_unique ON Pokemon(pokedex_number, region_id);

COMMIT TRANSACTION;