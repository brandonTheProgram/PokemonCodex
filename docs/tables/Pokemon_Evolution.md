# Pokemon_Evolution

**Rows:** 513

## Columns

| # | name | type | pk | notnull | default |
|---:|-----|------|---:|--------:|---------|
| 0 | base_pokedex_number | INTEGER | 1 | 1 |  |
| 1 | evolved_pokedex_number | INTEGER | 3 | 1 |  |
| 2 | base_region_id | INTEGER | 2 | 0 |  |
| 3 | evolved_region_id | INTEGER | 4 | 0 |  |
| 4 | evolution_condition | TEXT | 0 | 1 |  |
| 5 | chain_id | INTEGER | 5 | 1 |  |

## Foreign Keys
| id | seq | from | to | table | on_update | on_delete | match |
|---:|----:|------|----|-------|-----------|-----------|-------|
| 0 | 0 | evolved_pokedex_number | pokedex_number | Pokemon | NO ACTION | NO ACTION | NONE |
| 0 | 1 | evolved_region_id | region_id | Pokemon | NO ACTION | NO ACTION | NONE |
| 1 | 0 | base_pokedex_number | pokedex_number | Pokemon | NO ACTION | NO ACTION | NONE |
| 1 | 1 | base_region_id | region_id | Pokemon | NO ACTION | NO ACTION | NONE |

## Indexes
| seq | name | unique | origin | partial |
|----:|------|-------:|--------|---------|
| 0 | idx_pokemon_evolution_evolved | 0 | c | 0 |
| 1 | idx_pokemon_evolution_base | 0 | c | 0 |
| 2 | sqlite_autoindex_Pokemon_Evolution_1 | 1 | pk | 0 |

## Sample Rows
| base_pokedex_number | evolved_pokedex_number | base_region_id | evolved_region_id | evolution_condition | chain_id |
|---|---|---|---|---|---|
| 1 | 2 | None | None | Level 16 | 1 |
| 2 | 3 | None | None | Level 32 | 1 |
| 4 | 5 | None | None | Level 16 | 2 |
| 5 | 6 | None | None | Level 36 | 2 |
| 7 | 8 | None | None | Level 16 | 3 |
| 8 | 9 | None | None | Level 36 | 3 |
| 10 | 11 | None | None | Level 7 | 4 |
| 11 | 12 | None | None | Level 10 | 4 |
| 13 | 14 | None | None | Level 7 | 5 |
| 14 | 15 | None | None | Level 10 | 5 |
| 16 | 17 | None | None | Level 18 | 6 |
| 17 | 18 | None | None | Level 36 | 6 |
| 19 | 20 | None | None | Level 20 | 7 |
| 19 | 20 | 1 | 1 | Level 20 During Nighttime | 7 |
| 21 | 22 | None | None | Level 20 | 8 |
