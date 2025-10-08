# Pokemon_Move_Category

**Rows:** 3

## Columns

| # | name | type | pk | notnull | default |
|---:|-----|------|---:|--------:|---------|
| 0 | category_id | INTEGER | 1 | 0 |  |
| 1 | category_name | TEXT | 0 | 1 |  |

## Foreign Keys
_(none)_

## Indexes
| seq | name | unique | origin | partial |
|----:|------|-------:|--------|---------|
| 0 | idx_pokemon_move_category | 0 | c | 0 |
| 1 | sqlite_autoindex_Pokemon_Move_Category_1 | 1 | u | 0 |

## Sample Rows
| category_id | category_name |
|---|---|
| 1 | Physical |
| 2 | Special |
| 3 | Status |
