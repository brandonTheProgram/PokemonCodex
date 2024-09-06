-- Data Insertion for Branch Evolutions
INSERT INTO Evolution (base_pokedex_number, base_region_id, evolved_pokedex_number, evolved_region_id, evolution_method)
VALUES (79, NULL, 80, 1, 'Level 37'); -- Kantonian Slowpoke to Slowbro

INSERT INTO Evolution (base_pokedex_number, base_region_id, evolved_pokedex_number, evolved_region_id, evolution_method)
VALUES (79, 2, 80, 2, 'Galarian: Use Galarica Cuff'); -- Galarian Slowpoke to Galarian Slowbro

-- Recursive Query for evolutionary line using Pokedex Number
WITH RECURSIVE EvolutionChain AS (
    -- Base case: Find the Pokémon by its Pokédex number (ignore region)
    SELECT p.pokedex_number, p.region_id, p.name, e.base_pokedex_number, e.evolved_pokedex_number, e.evolution_method
    FROM Pokemon p
    LEFT JOIN Evolution e ON p.pokedex_number = e.base_pokedex_number
    WHERE p.pokedex_number = 496 -- Replace with the given Pokédex number (e.g., Servine)

    UNION

    -- Recursive step: Find all Pokémon that evolve from the current Pokémon (ignore region)
    SELECT p.pokedex_number, p.region_id, p.name, e.base_pokedex_number, e.evolved_pokedex_number, e.evolution_method
    FROM Pokemon p
    LEFT JOIN Evolution e ON p.pokedex_number = e.base_pokedex_number
    JOIN EvolutionChain ec ON p.pokedex_number = ec.evolved_pokedex_number

    UNION

    -- Reverse the process: Find all pre-evolutions (ignore region)
    SELECT p.pokedex_number, p.region_id, p.name, e.base_pokedex_number, e.evolved_pokedex_number, e.evolution_method
    FROM Pokemon p
    LEFT JOIN Evolution e ON p.pokedex_number = e.evolved_pokedex_number
    JOIN EvolutionChain ec ON p.pokedex_number = ec.base_pokedex_number
)

SELECT * FROM EvolutionChain;

--Populating the Pokemon Location Table
INSERT INTO Pokemon_Location (pokedex_number, region_id, game_id, location_name)
VALUES
(1, 1, 1, 'Route 1'),     -- Bulbasaur in Kanto Route 1 in Pokémon Red
(1, 1, 2, 'Route 2');     -- Bulbasaur in Kanto Route 2 in Pokémon Blue

--Populating the Move Table
INSERT INTO Move (move_name, type, category, power, accuracy, pp)
VALUES
('Thunderbolt', 'Electric', 'Special', 90, 100.00, 15),
('Tackle', 'Normal', 'Physical', 40, 100.00, 35),
('Flamethrower', 'Fire', 'Special', 90, 100.00, 15);

--Populating the Move Learn Table
INSERT INTO Move_Learn_Method (method_name)
VALUES
('Level Up'),
('TM'),
('HM'),
('Eggmove'),
('TR');

