BEGIN TRANSACTION;

INSERT INTO Pokemon_Regional_Form (region_name)
VALUES 
('Alolan'),
('Galarian'),
('Hisuian'),
('Paldean');

COMMIT TRANSACTION;