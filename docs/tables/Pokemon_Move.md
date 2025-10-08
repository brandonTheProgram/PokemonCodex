# Pokemon_Move

**Rows:** 840

## Columns

| # | name | type | pk | notnull | default |
|---:|-----|------|---:|--------:|---------|
| 0 | move_id | INTEGER | 1 | 0 |  |
| 1 | move_name | TEXT | 0 | 1 |  |
| 2 | description | TEXT | 0 | 1 |  |
| 3 | type_id | INTEGER | 0 | 1 |  |
| 4 | category_id | INTEGER | 0 | 0 |  |
| 5 | power | INTEGER | 0 | 0 |  |
| 6 | accuracy | FLOAT(5, 2) | 0 | 0 |  |
| 7 | pp | INTEGER | 0 | 0 |  |

## Foreign Keys
| id | seq | from | to | table | on_update | on_delete | match |
|---:|----:|------|----|-------|-----------|-----------|-------|
| 0 | 0 | category_id | category_id | Pokemon_Move_Category | NO ACTION | NO ACTION | NONE |
| 1 | 0 | type_id | type_id | Pokemon_Type | NO ACTION | NO ACTION | NONE |

## Indexes
| seq | name | unique | origin | partial |
|----:|------|-------:|--------|---------|
| 0 | idx_pokemon_move | 0 | c | 0 |
| 1 | sqlite_autoindex_Pokemon_Move_1 | 1 | u | 0 |

## Sample Rows
| move_id | move_name | description | type_id | category_id | power | accuracy | pp |
|---|---|---|---|---|---|---|---|
| 1 | Absorb | A nutrient-draining attack. The user's HP is restored by up to half the damage taken by the target. | 12 | 2 | 20 | 100.0 | 25 |
| 2 | Accelerock | The user smashes into the target at high speed. This move always goes first. | 6 | 1 | 40 | 100.0 | 20 |
| 3 | Acid | The opposing Pokemon are attacked with a spray of harsh acid. This may also lower their Sp. Def stat. | 4 | 2 | 40 | 100.0 | 30 |
| 4 | Acid Armor | The user alters its cellular structure to liquefy itself, sharply raising its Defense stat. | 4 | 3 | 0 | 100.0 | 20 |
| 5 | Acid Spray | The user spits fluid that works to melt the target. This harshly lowers the target's Sp. Def stat. | 4 | 2 | 40 | 100.0 | 20 |
| 6 | Acrobatics | The user nimbly strikes the target. If the user is not holding an item, this attack inflicts massive damage. | 1 | 1 | 55 | 100.0 | 15 |
| 7 | Acupressure | The user applies pressure to stress points, sharply boosting one of its or its allies' stats. | 1 | 3 | 0 | 100.0 | 30 |
| 8 | Aerial Ace | The user confounds the target with speed, then slashes. This attack never misses. | 3 | 1 | 60 | 100.0 | 20 |
| 9 | Aeroblast | A vortex of air is shot at the target to inflict damage. Critical hits land more easily. | 3 | 2 | 100 | 95.0 | 5 |
| 10 | After You | The user helps the target and makes it use its move right after the user. | 1 | 3 | 0 | 100.0 | 15 |
| 11 | Agility | The user relaxes and lightens its body to move faster. This sharply raises the Speed stat. | 14 | 3 | 0 | 10.0 | 30 |
| 12 | Air Cutter | The user launches razor-like wind to slash the opposing Pokemon. Critical hits land more easily. | 3 | 2 | 60 | 95.0 | 25 |
| 13 | Air Slash | The user attacks with a blade of air that slices even the sky. This may also make the target flinch. | 3 | 2 | 75 | 95.0 | 15 |
| 14 | Alluring Voice | The user attacks the target using its angelic voice. This also confuses the target if its stats have been boosted during the turn. | 18 | 2 | 80 | 100.0 | 10 |
| 15 | Ally Switch | The user teleports using a strange power and switches places with one of its allies. | 14 | 3 | 0 | 100.0 | 15 |
