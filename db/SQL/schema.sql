BEGIN TRANSACTION;

/*******
Regional Forms Table

Store the unique region/form identifies.
*******/
CREATE TABLE IF NOT EXISTS Regional_Form (
    region_id INT PRIMARY KEY AUTOINCREMENT,
    region_name TEXT NOT NULL UNIQUE -- e.g., 'Alola', 'Galar', 'Hisui', 'Paldea'
);

/*******
Ability Table

Store the abilitity a Pokémon can have.
*******/
CREATE TABLE IF NOT EXISTS Ability (
    ability_id INT PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL UNIQUE,
    description TEXT NOT NULL
);

/*******
Available Game Table

Store the unique game identifies.
*******/
CREATE TABLE IF NOT EXISTS Game (
    game_id INT PRIMARY KEY AUTOINCREMENT,     -- Unique identifier for each game
    game_name TEXT NOT NULL UNIQUE             -- The name of the game (e.g., Red, Blue, Sword, Shield)
);

/*******
Move Table

Store a move a Pokemon can learn.
*******/
CREATE TABLE IF NOT EXISTS Move (
    move_id INT PRIMARY KEY AUTOINCREMENT, -- Unique identifier for the move
    move_name TEXT NOT NULL UNIQUE,        -- Name of the move (e.g., "Thunderbolt")
    description TEXT NOT NULL,
    type TEXT NOT NULL,                    -- Type of the move (e.g., Electric, Water)
    category TEXT,                         -- Category of the move (Physical, Special, Status)
    power INT,                             -- Base power of the move (e.g., 90 for Thunderbolt)
    accuracy FLOAT(5, 2),                  -- Accuracy percentage (e.g., 100.00 for Thunderbolt)
    pp INT                                 -- Power Points for the move
);

/*******
Move Learn Method Table

Store how a Pokemon can learn the move.
*******/
CREATE TABLE IF NOT EXISTS Move_Learn_Method (
    method_id INT PRIMARY KEY AUTOINCREMENT, -- Unique identifier for the learn method
    method_name TEXT NOT NULL UNIQUE         -- Name of the method (e.g., "Level Up", "TM", "HM", "Eggmove", "TR")
);

/*******
Pokemon Table

Stores basic information about each Pokémon.
A junction table to link Pokémon with their regional forms.
*******/
CREATE TABLE IF NOT EXISTS Pokemon (
    pokedex_number INT NOT NULL,
    region_id INT, -- References the region in the Regional_Form table. NULL for a Pokemon not being a regional variant
    name TEXT NOT NULL,
    image TEXT NOT NULL,
    species TEXT NOT NULL,
    pri_type TEXT NOT NULL,
    sec_type TEXT,
    description TEXT NOT NULL,

    -- Key Constraints
    PRIMARY KEY (pokedex_number, region_id),

    FOREIGN KEY (region_id) REFERENCES Regional_Form(region_id)
);

/*******
Pokemon Ability Table

Stores the information about the Pokemon's ability
A junction table to link Pokémon with their abilities.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Ability (
    pokedex_number INT NOT NULL,
    region_id INT,
    ability_id INT,

    -- Key Constraints
    PRIMARY KEY (pokedex_number, region_id, ability_id),

    FOREIGN KEY (pokedex_number, region_id) REFERENCES Pokemon(pokedex_number, region_id),
    FOREIGN KEY (ability_id) REFERENCES Abilities(ability_id)
);

/*******
Evolution Table

Stores a Pokemon's evolution information.
*******/
CREATE TABLE IF NOT EXISTS Evolution (
    evolution_id INT PRIMARY KEY AUTOINCREMENT,
    base_pokedex_number INT NOT NULL, -- The Pokémon before evolution
    base_region_id INT, -- The region/form of the base Pokémon
    evolved_pokedex_number INT NOT NULL, -- The Pokémon after evolution
    evolved_region_id INT, -- The region/form of the evolved Pokémon
    evolution_method TEXT, -- Method of evolution (e.g., level-up, item, trade, etc.)

    -- Key Constraints
    FOREIGN KEY (base_pokedex_number, base_region_id) REFERENCES Pokemon(pokedex_number, region_id),
    FOREIGN KEY (evolved_pokedex_number, evolved_region_id) REFERENCES Pokemon(pokedex_number, region_id)
);

/*******
Pokemon Location Table

Stores the location where the Pokemoin is found in each game.
A junction table to link Pokémon with their game locations.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Location (
    location_id PRIMARY KEY AUTOINCREMENT,     -- Unique identifier for each location entry
    pokedex_number INT NOT NULL,               -- Reference to the Pokémon's Pokédex number
    region_id INT,                             -- Reference to the Pokémon's regional form
    game_id INT NOT NULL,                      -- Reference to the game (Red, Blue, Sword, Shield, etc.)
    location_name TEXT NOT NULL,               -- The name of the location(s) (Route 1, Viridian Forest, etc.)
    
    -- Key Constraints
    FOREIGN KEY (pokedex_number, region_id) REFERENCES Pokemon(pokedex_number, region_id),
    FOREIGN KEY (game_id) REFERENCES Game(game_id)
);

/*******
Pokemon Moveset Table

Stores the moveset of the Pokemon for each game.
A junction table to link Pokémon with their game locations.
*******/
CREATE TABLE IF NOT EXISTS Pokemon_Moveset (
    pokedex_number INT NOT NULL,           -- Reference to the Pokémon's Pokédex number
    region_id INT,                         -- Reference to the Pokémon's region (for regional forms)
    move_id INT NOT NULL,                  -- Reference to the move
    method_id INT NOT NULL,                -- Reference to how the move is learned
    level_learned INT,                     -- Level at which the move is learned (relevant for Level Up method)
    technical_number INT,                  -- TM/TR number (relevant for TM, TR, etc)
    game_id INT NOT NULL,                  -- Reference to the specific game (Red, Blue, Sword, etc.)

    -- Key Constraints
    PRIMARY KEY (pokedex_number, region_id, move_id, method_id, game_id),

    FOREIGN KEY (pokedex_number, region_id) REFERENCES Pokemon(pokedex_number, region_id),
    FOREIGN KEY (move_id) REFERENCES Move(move_id),
    FOREIGN KEY (method_id) REFERENCES Move_Learn_Method(method_id),
    FOREIGN KEY (game_id) REFERENCES Game(game_id)
);

COMMIT TRANSACTION;