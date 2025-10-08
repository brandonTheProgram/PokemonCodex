# Pokemon

**Rows:** 1080

## Columns

| # | name | type | pk | notnull | default |
|---:|-----|------|---:|--------:|---------|
| 0 | pokedex_number | INTEGER | 1 | 1 |  |
| 1 | region_id | INTEGER | 2 | 0 |  |
| 2 | name | TEXT | 0 | 1 |  |
| 3 | image | TEXT | 0 | 1 |  |
| 4 | species | TEXT | 0 | 1 |  |
| 5 | primary_type_id | INTEGER | 0 | 1 |  |
| 6 | secondary_type_id | INTEGER | 0 | 0 |  |
| 7 | primary_ability_id | INTEGER | 0 | 1 |  |
| 8 | secondary_ability_id | INTEGER | 0 | 0 |  |
| 9 | hidden_ability_id | INTEGER | 0 | 0 |  |
| 10 | description | TEXT | 0 | 1 |  |

## Foreign Keys
| id | seq | from | to | table | on_update | on_delete | match |
|---:|----:|------|----|-------|-----------|-----------|-------|
| 0 | 0 | region_id | region_id | Pokemon_Regional_Form | NO ACTION | NO ACTION | NONE |
| 1 | 0 | hidden_ability_id | ability_id | Pokemon_Ability | NO ACTION | NO ACTION | NONE |
| 2 | 0 | secondary_ability_id | ability_id | Pokemon_Ability | NO ACTION | NO ACTION | NONE |
| 3 | 0 | primary_ability_id | ability_id | Pokemon_Ability | NO ACTION | NO ACTION | NONE |
| 4 | 0 | secondary_type_id | type_id | Pokemon_Type | NO ACTION | NO ACTION | NONE |
| 5 | 0 | primary_type_id | type_id | Pokemon_Type | NO ACTION | NO ACTION | NONE |

## Indexes
| seq | name | unique | origin | partial |
|----:|------|-------:|--------|---------|
| 0 | idx_region_pokemon | 0 | c | 0 |
| 1 | sqlite_autoindex_Pokemon_1 | 1 | pk | 0 |

## Sample Rows
| pokedex_number | region_id | name | image | species | primary_type_id | secondary_type_id | primary_ability_id | secondary_ability_id | hidden_ability_id | description |
|---|---|---|---|---|---|---|---|---|---|---|
| 1 | None | Bulbasaur | https://i.imgur.com/uxemRFU.png | Seed Pokémon | 12 | 4 | 161 | None | 27 | For some time after its birth, it grows by taking nourishment from the seed on its back. |
| 2 | None | Ivysaur | https://i.imgur.com/vlqnvvP.png | Seed Pokémon | 12 | 4 | 161 | None | 27 | When the bud on its back starts swelling, a sweet aroma wafts to indicate the flower's coming bloom. |
| 3 | None | Venusaur | https://i.imgur.com/jledOia.png | Seed Pokémon | 12 | 4 | 161 | None | 27 | After a rainy day, the flower on its back smells stronger. The scent attracts other Pokémon. |
| 4 | None | Charmander | https://i.imgur.com/ZjYCxIe.png | Lizard Pokémon | 10 | None | 23 | None | 231 | The fire on the tip of its tail is a measure of its life. If the Pokémon is healthy, its tail burns intensely. |
| 5 | None | Charmeleon | https://i.imgur.com/r6lTUIf.png | Flame Pokémon | 10 | None | 23 | None | 231 | In the rocky mountains where Charmeleon live, their fiery tails shine at night like stars. |
| 6 | None | Charizard | https://i.imgur.com/IiRtPgN.png | Flame Pokémon | 10 | 3 | 23 | None | 231 | It is said that Charizard's fire burns hotter if it has experienced harsh battles. |
| 7 | None | Squirtle | https://i.imgur.com/mg6oWKo.png | Tiny Turtle Pokémon | 11 | None | 274 | None | 192 | It hides in its shell to protect itself, then strikes back with spouts of water at every opportunity. |
| 8 | None | Wartortle | https://i.imgur.com/K7okE5l.png | Turtle Pokémon | 11 | None | 274 | None | 192 | It is said to live 10,000 years. Its furry tail is popular as a symbol of longevity. |
| 9 | None | Blastoise | https://i.imgur.com/Pkvi7yT.png | Shellfish Pokémon | 11 | None | 274 | None | 192 | The jets of water it spouts from the rocket cannons on its shell can punch through thick steel. |
| 10 | None | Caterpie | https://i.imgur.com/jZslsxi.png | Worm Pokémon | 7 | None | 222 | None | 204 | It releases a stench from its red antennae to repel enemies. It grows by molting repeatedly. |
| 11 | None | Metapod | https://i.imgur.com/ekSwReX.png | Cocoon Pokémon | 7 | None | 219 | None | None | A steel-hard shell protects its tender body. The Pokémon quietly endures hardships while awaiting evolution. |
| 12 | None | Butterfree | https://i.imgur.com/NgHmrzG.png | Butterfly Pokémon | 7 | 3 | 34 | None | 273 | It loves the nectar of flowers and can locate flower patches that have even tiny amounts of pollen. |
| 13 | None | Weedle | https://i.imgur.com/SgU7gz6.png | Hairy Bug Pokémon | 7 | 4 | 222 | None | 204 | It eats its weight in leaves every day. It fends off attackers with the needle on its head. |
| 14 | None | Kakuna | https://i.imgur.com/FAotGnz.png | Cocoon Pokémon | 7 | 4 | 219 | None | None | While awaiting evolution, it hides from predators under leaves and in nooks of branches. |
| 15 | None | Beedrill | https://i.imgur.com/FCts0yD.png | Poison Bee Pokémon | 7 | 4 | 256 | None | 228 | Its best attack involves flying around at high speed, striking with poison needles, then flying off. |
