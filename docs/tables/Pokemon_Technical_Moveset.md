# Pokemon_Technical_Moveset

**Rows:** 62488

## Columns

| # | name | type | pk | notnull | default |
|---:|-----|------|---:|--------:|---------|
| 0 | technical_moveset_id | INTEGER | 1 | 0 |  |
| 1 | pokedex_number | INTEGER | 0 | 1 |  |
| 2 | region_id | INTEGER | 0 | 0 |  |
| 3 | mainline_game_id | INTEGER | 0 | 1 |  |
| 4 | technical_move_id | INTEGER | 0 | 1 |  |

## Foreign Keys
| id | seq | from | to | table | on_update | on_delete | match |
|---:|----:|------|----|-------|-----------|-----------|-------|
| 0 | 0 | mainline_game_id | mainline_game_id | Pokemon_Mainline_Game | NO ACTION | NO ACTION | NONE |
| 1 | 0 | pokedex_number | pokedex_number | Pokemon | NO ACTION | NO ACTION | NONE |
| 1 | 1 | region_id | region_id | Pokemon | NO ACTION | NO ACTION | NONE |

## Indexes
_(none)_

## Sample Rows
| technical_moveset_id | pokedex_number | region_id | mainline_game_id | technical_move_id |
|---|---|---|---|---|
| 1 | 1 | None | 1 | 11 |
| 2 | 1 | None | 1 | 12 |
| 3 | 1 | None | 1 | 18 |
| 4 | 1 | None | 1 | 20 |
| 5 | 1 | None | 1 | 22 |
| 6 | 1 | None | 1 | 25 |
| 7 | 1 | None | 1 | 26 |
| 8 | 1 | None | 1 | 29 |
| 9 | 1 | None | 1 | 30 |
| 10 | 1 | None | 1 | 32 |
| 11 | 1 | None | 1 | 35 |
| 12 | 1 | None | 1 | 40 |
| 13 | 1 | None | 1 | 42 |
| 14 | 1 | None | 1 | 47 |
| 15 | 1 | None | 1 | 51 |
