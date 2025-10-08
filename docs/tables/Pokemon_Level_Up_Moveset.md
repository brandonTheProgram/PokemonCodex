# Pokemon_Level_Up_Moveset

**Rows:** 23724

## Columns

| # | name | type | pk | notnull | default |
|---:|-----|------|---:|--------:|---------|
| 0 | level_up_moveset_id | INTEGER | 1 | 0 |  |
| 1 | pokedex_number | INTEGER | 0 | 1 |  |
| 2 | region_id | INTEGER | 0 | 0 |  |
| 3 | move_id | INTEGER | 0 | 1 |  |
| 4 | mainline_game_id | INTEGER | 0 | 1 |  |
| 5 | level_learned | INTEGER | 0 | 1 |  |

## Foreign Keys
| id | seq | from | to | table | on_update | on_delete | match |
|---:|----:|------|----|-------|-----------|-----------|-------|
| 0 | 0 | mainline_game_id | mainline_game_id | Pokemon_Mainline_Game | NO ACTION | NO ACTION | NONE |
| 1 | 0 | move_id | move_id | Pokemon_Move | NO ACTION | NO ACTION | NONE |
| 2 | 0 | pokedex_number | pokedex_number | Pokemon | NO ACTION | NO ACTION | NONE |
| 2 | 1 | region_id | region_id | Pokemon | NO ACTION | NO ACTION | NONE |

## Indexes
_(none)_

## Sample Rows
| level_up_moveset_id | pokedex_number | region_id | move_id | mainline_game_id | level_learned |
|---|---|---|---|---|---|
| 1 | 1 | None | 739 | 1 | 1 |
| 2 | 1 | None | 303 | 1 | 1 |
| 3 | 1 | None | 804 | 1 | 3 |
| 4 | 1 | None | 304 | 1 | 6 |
| 5 | 1 | None | 399 | 1 | 9 |
| 6 | 1 | None | 565 | 1 | 12 |
| 7 | 1 | None | 515 | 1 | 15 |
| 8 | 1 | None | 653 | 1 | 15 |
| 9 | 1 | None | 617 | 1 | 18 |
| 10 | 1 | None | 744 | 1 | 21 |
| 11 | 1 | None | 731 | 1 | 24 |
| 12 | 1 | None | 736 | 1 | 27 |
| 13 | 1 | None | 832 | 1 | 30 |
| 14 | 1 | None | 170 | 1 | 33 |
| 15 | 1 | None | 671 | 1 | 36 |
