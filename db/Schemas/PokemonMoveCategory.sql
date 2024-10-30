BEGIN TRANSACTION;

INSERT INTO Pokemon_Move_Category (category_name) 
VALUES
('Physical'), 
('Special'), 
('Status');

COMMIT TRANSACTION;