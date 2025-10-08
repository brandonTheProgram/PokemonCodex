# Pokemon_Type_Effectiveness

**Rows:** 324

## Columns

| # | name | type | pk | notnull | default |
|---:|-----|------|---:|--------:|---------|
| 0 | type_effectiveness_id | INTEGER | 1 | 0 |  |
| 1 | attacking_type_id | INTEGER | 0 | 1 |  |
| 2 | defending_type_id | INTEGER | 0 | 1 |  |
| 3 | damage_multiplier | REAL | 0 | 1 |  |

## Foreign Keys
| id | seq | from | to | table | on_update | on_delete | match |
|---:|----:|------|----|-------|-----------|-----------|-------|
| 0 | 0 | defending_type_id | type_id | Pokemon_Type | NO ACTION | NO ACTION | NONE |
| 1 | 0 | attacking_type_id | type_id | Pokemon_Type | NO ACTION | NO ACTION | NONE |

## Indexes
_(none)_

## Sample Rows
| type_effectiveness_id | attacking_type_id | defending_type_id | damage_multiplier |
|---|---|---|---|
| 1 | 1 | 1 | 1.0 |
| 2 | 1 | 2 | 2.0 |
| 3 | 1 | 3 | 1.0 |
| 4 | 1 | 4 | 1.0 |
| 5 | 1 | 5 | 1.0 |
| 6 | 1 | 6 | 1.0 |
| 7 | 1 | 7 | 1.0 |
| 8 | 1 | 8 | 0.0 |
| 9 | 1 | 9 | 1.0 |
| 10 | 1 | 10 | 1.0 |
| 11 | 1 | 11 | 1.0 |
| 12 | 1 | 12 | 1.0 |
| 13 | 1 | 13 | 1.0 |
| 14 | 1 | 14 | 1.0 |
| 15 | 1 | 15 | 1.0 |
