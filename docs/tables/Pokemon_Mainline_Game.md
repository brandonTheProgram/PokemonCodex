# Pokemon_Mainline_Game

**Rows:** 3

## Columns

| # | name | type | pk | notnull | default |
|---:|-----|------|---:|--------:|---------|
| 0 | mainline_game_id | INTEGER | 1 | 0 |  |
| 1 | game_name | TEXT | 0 | 1 |  |

## Foreign Keys
_(none)_

## Indexes
| seq | name | unique | origin | partial |
|----:|------|-------:|--------|---------|
| 0 | idx_pokemon_mainline_game | 0 | c | 0 |
| 1 | sqlite_autoindex_Pokemon_Mainline_Game_1 | 1 | u | 0 |

## Sample Rows
| mainline_game_id | game_name |
|---|---|
| 1 | Sword & Shield |
| 2 | Legends: Arceus |
| 3 | Scarlet & Violet |
