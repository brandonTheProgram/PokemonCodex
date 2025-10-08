# Pokemon_Location

**Rows:** 5400

## Columns

| # | name | type | pk | notnull | default |
|---:|-----|------|---:|--------:|---------|
| 0 | pokedex_number | INTEGER | 1 | 1 |  |
| 1 | region_id | INTEGER | 2 | 0 |  |
| 2 | game_id | INTEGER | 3 | 1 |  |
| 3 | location_name | TEXT | 0 | 0 |  |

## Foreign Keys
| id | seq | from | to | table | on_update | on_delete | match |
|---:|----:|------|----|-------|-----------|-----------|-------|
| 0 | 0 | game_id | game_id | Pokemon_Game | NO ACTION | NO ACTION | NONE |
| 1 | 0 | pokedex_number | pokedex_number | Pokemon | NO ACTION | NO ACTION | NONE |
| 1 | 1 | region_id | region_id | Pokemon | NO ACTION | NO ACTION | NONE |

## Indexes
| seq | name | unique | origin | partial |
|----:|------|-------:|--------|---------|
| 0 | sqlite_autoindex_Pokemon_Location_1 | 1 | pk | 0 |

## Sample Rows
| pokedex_number | region_id | game_id | location_name |
|---|---|---|---|
| 1 | None | 1 | Gift in Master Dojo |
| 1 | None | 2 | Gift in Master Dojo |
| 1 | None | 3 | None |
| 1 | None | 4 | Coastal Biome, Torchlit Labyrinth |
| 1 | None | 5 | Coastal Biome, Torchlit Labyrinth |
| 2 | None | 1 | Dynamax Adventures |
| 2 | None | 2 | Dynamax Adventures |
| 2 | None | 3 | None |
| 2 | None | 4 | Evolve Bulbasaur |
| 2 | None | 5 | Evolve Bulbasaur |
| 3 | None | 1 | Max Raid Battles: Forest of Focus, Training Lowlands<br>Gigantamax Raid Battles: Forest of Focus, Training Lowlands |
| 3 | None | 2 | Max Raid Battles: Forest of Focus, Training Lowlands<br>Gigantamax Raid Battles: Forest of Focus, Training Lowlands |
| 3 | None | 3 | None |
| 3 | None | 4 | Evolve Ivysaur |
| 3 | None | 5 | Evolve Ivysaur |
