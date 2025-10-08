# Pokemon_Technical_Move

**Rows:** 429

## Columns

| # | name | type | pk | notnull | default |
|---:|-----|------|---:|--------:|---------|
| 0 | technical_move_id | INTEGER | 1 | 0 |  |
| 1 | move_id | INTEGER | 0 | 1 |  |
| 2 | mainline_game_id | INTEGER | 0 | 1 |  |
| 3 | technical_number | INTEGER | 0 | 1 |  |
| 4 | is_tr | INTEGER | 0 | 1 |  |

## Foreign Keys
| id | seq | from | to | table | on_update | on_delete | match |
|---:|----:|------|----|-------|-----------|-----------|-------|
| 0 | 0 | mainline_game_id | mainline_game_id | Pokemon_Mainline_Game | NO ACTION | NO ACTION | NONE |
| 1 | 0 | move_id | move_id | Pokemon_Move | NO ACTION | NO ACTION | NONE |

## Indexes
_(none)_

## Sample Rows
| technical_move_id | move_id | mainline_game_id | technical_number | is_tr |
|---|---|---|---|---|
| 1 | 435 | 1 | 0 | 0 |
| 2 | 434 | 1 | 1 | 0 |
| 3 | 498 | 1 | 2 | 0 |
| 4 | 240 | 1 | 3 | 0 |
| 5 | 360 | 1 | 4 | 0 |
| 6 | 766 | 1 | 5 | 0 |
| 7 | 261 | 1 | 6 | 0 |
| 8 | 507 | 1 | 7 | 0 |
| 9 | 348 | 1 | 8 | 0 |
| 10 | 289 | 1 | 9 | 0 |
| 11 | 420 | 1 | 10 | 0 |
| 12 | 671 | 1 | 11 | 0 |
| 13 | 672 | 1 | 12 | 0 |
| 14 | 241 | 1 | 13 | 0 |
| 15 | 768 | 1 | 14 | 0 |
