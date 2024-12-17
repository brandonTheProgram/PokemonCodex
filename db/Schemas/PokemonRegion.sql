BEGIN TRANSACTION;
INSERT INTO Pokemon_Region (name, image, start, end)
VALUES 
("Kanto", "https://i.imgur.com/YLK9mk6.png", 1, 151),
("Johto", "https://i.imgur.com/uOfw3yK.png", 152, 251),
("Hoenn", "https://i.imgur.com/ssgo9L9.png", 252, 386),
("Sinnoh", "https://i.imgur.com/cKe2R7l.png", 387, 493),
("Unova", "https://i.imgur.com/m6OSz1K.png", 494, 649),
("Kalos", "https://i.imgur.com/Oxknl4S.png", 650, 721),
("Alola", "https://i.imgur.com/idT172R.png", 722, 809),
("Galar", "https://i.imgur.com/PBYJCjg.png", 810, 905),
("Paldea", "https://i.imgur.com/Z0aWTGN.png", 906, 1025),
("All", "https://i.imgur.com/8R1kHGC.png", 1, 1025);

COMMIT TRANSACTION;