BEGIN TRANSACTION;

INSERT INTO Pokemon_Game (game_name)
VALUES 
('Sword'),
('Shield'),
('Legends: Arceus'),
('Scarlet'),
('Violet');

COMMIT TRANSACTION;