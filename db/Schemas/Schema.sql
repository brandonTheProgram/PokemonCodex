BEGIN TRANSACTION;

/*******
Pokémon Type Table

Store the unique Pokémon type identifiers.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Type (
    type_id INTEGER PRIMARY KEY AUTOINCREMENT,
    type_name TEXT NOT NULL UNIQUE -- e.g., 'Electric', 'Water', 'Fire'
);

/*******
Pokémon Move Category Table

Store the unique Pokémon move category identifiers.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Move_Category (
    category_id INTEGER PRIMARY KEY AUTOINCREMENT,
    category_name TEXT NOT NULL UNIQUE -- e.g., 'Physical', 'Special', 'Status'
);

/*******
Pokémon Regional Forms Table

Store the unique Pokémon region/form identifiers.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Regional_Form (
    region_id INTEGER PRIMARY KEY AUTOINCREMENT,
    region_name TEXT NOT NULL UNIQUE -- e.g., 'Alola', 'Galar', 'Hisui', 'Paldea'
);

/*******
Pokémon Ability Table

Store the unique Pokémon abilitity identifiers.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Ability (
    ability_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    description TEXT NOT NULL
);

/*******
Pokémon Game Table

Store the unique Pokémon available game identifiers.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Game (
    game_id INTEGER PRIMARY KEY AUTOINCREMENT, -- Unique identifier for each game
    game_name TEXT NOT NULL UNIQUE             -- The name of the game (e.g., Red, Blue, Sword, Shield)
);

/*******
Pokémon Move Table

Store a move a Pokémon can learn.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Move (
    move_id INTEGER PRIMARY KEY AUTOINCREMENT,  -- Unique identifier for the move
    move_name TEXT NOT NULL UNIQUE,             -- Name of the move (e.g., "Thunderbolt")
    description TEXT NOT NULL,                  -- Description of the move
    type_id INTEGER NOT NULL,                   -- Reference to the move's type (from Pokemon_Type table)
    category_id INTEGER,                        -- Reference to the move's category (from Pokemon_Move_Category table)
    power INTEGER,                              -- Base power of the move (e.g., 90 for Thunderbolt)
    accuracy FLOAT(5, 2),                       -- Accuracy percentage (e.g., 100.00 for Thunderbolt)
    pp INTEGER,                                 -- Power Points for the move

    -- Key Constraints
    FOREIGN KEY (type_id) REFERENCES Pokemon_Type(type_id),
    FOREIGN KEY (category_id) REFERENCES Pokemon_Move_Category(category_id)
);

/*******
Pokémon Table

Stores basic information about each Pokémon.
*******/
CREATE TABLE IF NOT EXISTS Pokemon (
    pokedex_number INTEGER NOT NULL,        -- National Pokédex number
    region_id INTEGER,                      -- References the region in the Pokemon_Regional_Form table. NULL for a Pokémon not being a regional variant
    name TEXT NOT NULL,                     -- Pokémon's name
    image TEXT NOT NULL,                    -- Pokémon's image
    species TEXT NOT NULL,                  -- Pokémon's species
    primary_type_id INTEGER NOT NULL,       -- Reference to the Pokémon's primary type (from Pokemon_Move_Type table)
    secondary_type_id INTEGER,              -- Reference to the Pokémon's secondary type, if any (from Pokemon_Move_Type table)
    primary_ability_id INTEGER NOT NULL,    -- Reference to the Pokémon's primary ability (from Pokemon_Ability table)
    secondary_ability_id INTEGER,           -- Reference to the Pokémon's secondary ability, if any (from Pokemon_Ability table)
    hidden_ability_id INTEGER,              -- Reference to the Pokémon's hidden ability, if any (from Pokemon_Ability table)
    description TEXT NOT NULL,              -- Pokédex description

    -- Key Constraints
    PRIMARY KEY (pokedex_number, region_id),
    FOREIGN KEY (primary_type_id) REFERENCES Pokemon_Type(type_id),
    FOREIGN KEY (secondary_type_id) REFERENCES Pokemon_Type(type_id),
    FOREIGN KEY (primary_ability_id) REFERENCES Pokemon_Ability(ability_id),
    FOREIGN KEY (secondary_ability_id) REFERENCES Pokemon_Ability(ability_id),
    FOREIGN KEY (hidden_ability_id) REFERENCES Pokemon_Ability(ability_id),
    FOREIGN KEY (region_id) REFERENCES Pokemon_Regional_Form(region_id)
);

/*******
Evolution Table

Stores a Pokémon's evolution information.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Evolution (
    base_pokedex_number INTEGER NOT NULL,       -- The Pokémon before evolution
    evolved_pokedex_number INTEGER NOT NULL,    -- The Pokémon after evolution
    base_region_id INTEGER,                     -- References the region in the Pokemon_Regional_Form table. NULL for a Pokémon not being a regional variant
    evolved_region_id INTEGER,                  -- References the region in the Pokemon_Regional_Form table. NULL for a Pokémon not being a regional variant
    evolution_condition TEXT NOT NULL,          -- The condition on how the Pokemon evolves
    
    -- Key Constraints
    PRIMARY KEY (base_pokedex_number, base_region_id, evolved_pokedex_number, evolved_region_id) REFERENCES Pokemon(pokedex_number, region_id, pokedex_number, region_id)
    FOREIGN KEY (base_pokedex_number, base_region_id) REFERENCES Pokemon(pokedex_number, region_id),
    FOREIGN KEY (evolved_pokedex_number, evolved_region_id) REFERENCES Pokemon(pokedex_number, region_id)
);

/*******
Pokémon Location Table

Stores the location where the Pokémon is found in each game.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Location (
    pokedex_number INTEGER NOT NULL,               -- Reference to the Pokémon's Pokédex number
    region_id INTEGER,                             -- References the region in the Pokemon_Regional_Form table. NULL for a Pokémon not being a regional variant
    game_id INTEGER NOT NULL,                      -- References the game in the Pokemon_Game table. NULL for a Pokémon not being a regional variant
    location_name TEXT NOT NULL,                   -- The name of the location(s) (Route 1, Viridian Forest, etc.)
    
    -- Key Constraints
    PRIMARY KEY (pokedex_number, region_id) REFERENCES Pokemon(pokedex_number, region_id),
    FOREIGN KEY (game_id) REFERENCES Pokemon_Game(game_id)
);

/*******
Pokémon Moveset Table

Store the movesets for each Pokémon in a specific game along with learning method details.
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
    FOREIGN KEY (move_id) REFERENCES Pokemon_Move(move_id),
    FOREIGN KEY (game_id) REFERENCES Pokemon_Game(game_id)
);

CREATE TABLE IF NOT EXISTS Pokemon_Moveset (
    pokedex_number INTEGER NOT NULL,       -- Reference to the Pokémon's Pokédex number
    region_id INTEGER,                     -- References the region in the Pokemon_Regional_Form table. NULL for a Pokémon not being a regional variant
    move_id INTEGER NOT NULL,              -- Reference to the move in the Pokemon_Move table
    game_id INTEGER NOT NULL,              -- Reference to the game in the Pokemon_Game table
    learn_method TEXT NOT NULL,            -- Method of learning the move (e.g., "Level Up", "TM", "Egg Move")
    learn_parameter INTEGER,               -- Parameter for the learning method (level for leveling up, TM number, etc.)
    
    -- Key Constraints
    PRIMARY KEY (pokedex_number, region_id, move_id, game_id),
    FOREIGN KEY (pokedex_number, region_id) REFERENCES Pokemon(pokedex_number, region_id),
    FOREIGN KEY (move_id) REFERENCES Pokemon_Move(move_id),
    FOREIGN KEY (game_id) REFERENCES Pokemon_Game(game_id)
);

/*******
Indexing
*******/
-- Index on single columns
CREATE INDEX idx_pokemon_type ON Pokemon_Type(type_id);
CREATE INDEX idx_pokemon_move_category ON Pokemon_Move_Category(category_id);
CREATE INDEX idx_pokemon_regional_form ON Pokemon_Regional_Form(region_id);
CREATE INDEX idx_pokemon_ability ON Pokemon_Ability(ability_id);
CREATE INDEX idx_pokemon_game ON Pokemon_Game(game_id);
CREATE INDEX idx_pokemon_move ON Pokemon_Move(move_id);

-- Compound indexes
CREATE INDEX idx_pokemon_evolution_base ON Pokemon_Evolution(base_pokedex_number, base_region_id);
CREATE INDEX idx_pokemon_evolution_evolved ON Pokemon_Evolution(evolved_pokedex_number, evolved_region_id);
CREATE INDEX idx_pokemon_location ON Pokemon_Location(pokedex_number, region_id, game_id );
CREATE INDEX idx_pokemon_moveset_pokedex_region_move ON Pokemon_Moveset(pokedex_number, region_id, move_id, game_id);

-- Unique index for pokedex_number and region_id combination
CREATE UNIQUE INDEX idx_pokemon_pokedex_region_unique ON Pokemon(pokedex_number, region_id);

COMMIT TRANSACTION;