# Pokemon_Region

**Rows:** 10

## Columns

| # | name | type | pk | notnull | default |
|---:|-----|------|---:|--------:|---------|
| 0 | region_id | INTEGER | 1 | 0 |  |
| 1 | name | TEXT | 0 | 1 |  |
| 2 | image | TEXT | 0 | 1 |  |
| 3 | start | INTEGER | 0 | 1 |  |
| 4 | end | INTEGER | 0 | 1 |  |

## Foreign Keys
_(none)_

## Indexes
| seq | name | unique | origin | partial |
|----:|------|-------:|--------|---------|
| 0 | sqlite_autoindex_Pokemon_Region_1 | 1 | u | 0 |

## Sample Rows
| region_id | name | image | start | end |
|---|---|---|---|---|
| 1 | Kanto | https://i.imgur.com/YLK9mk6.png | 1 | 151 |
| 2 | Johto | https://i.imgur.com/uOfw3yK.png | 152 | 251 |
| 3 | Hoenn | https://i.imgur.com/ssgo9L9.png | 252 | 386 |
| 4 | Sinnoh | https://i.imgur.com/cKe2R7l.png | 387 | 493 |
| 5 | Unova | https://i.imgur.com/m6OSz1K.png | 494 | 649 |
| 6 | Kalos | https://i.imgur.com/Oxknl4S.png | 650 | 721 |
| 7 | Alola | https://i.imgur.com/idT172R.png | 722 | 809 |
| 8 | Galar | https://i.imgur.com/PBYJCjg.png | 810 | 905 |
| 9 | Paldea | https://i.imgur.com/Z0aWTGN.png | 906 | 1025 |
| 10 | All | https://i.imgur.com/J2Fo4iy.png | 1 | 1025 |
