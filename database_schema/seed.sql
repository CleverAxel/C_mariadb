DROP PROCEDURE IF EXISTS truncate_and_seed;
DELIMITER $$

CREATE PROCEDURE truncate_and_seed()
BEGIN
    SET FOREIGN_KEY_CHECKS = 0;

    TRUNCATE TABLE operations_supplies;
    TRUNCATE TABLE operations;
    TRUNCATE TABLE worksites;
    TRUNCATE TABLE estimations;
    TRUNCATE TABLE furnitures;
    TRUNCATE TABLE supplies;
    TRUNCATE TABLE craftmans;
    TRUNCATE TABLE clients;
    TRUNCATE TABLE logs;

    SET FOREIGN_KEY_CHECKS = 1;

    -- ======================
    -- CLIENTS
    -- ======================
    INSERT INTO clients (id, lastname, firstname, address, phone, category, account_balance)
    VALUES
    (1, 'Dupont', 'Jean', '12 Rue de Liege', '0412345678', 'idk', 120.50),
    (2, 'Martin', 'Claire', '45 Avenue de Bruxelles', '0498765432', 'idk', 2.00),
    (3, 'Bernard', 'Luc', '78 Rue des Fleurs', '0471122334', 'idk', 420.69);

    -- ======================
    -- CRAFTSMANS
    -- ======================
    INSERT INTO craftmans (registration_number, lastname, firstname, speciality)
    VALUES
    ('CR001', 'Leroy', 'Paul', 'serrurier'),
    ('CR002', 'Simon', 'Alice', 'mange de la colle'),
    ('CR003', 'Petit', 'Marc', 'bipolaire');

    -- ======================
    -- SUPPLIES
    -- ======================
    INSERT INTO supplies (id, label, stock_quantity, minimum_threshold, unit_cost, status)
    VALUES
    (1, 'Planche en chêne', 50, 10, 25.00, 'available'),
    (2, 'Vernis clair', 30, 5, 15.50, 'available'),
    (3, 'Cuir de vachette', 20, 5, 45.00, 'available'),
    (4, 'Colle forte', 100, 20, 5.00, 'available');

    -- ======================
    -- FURNITURES
    -- ======================
    INSERT INTO furnitures (id, client_id, furniture_type, style, description, status)
    VALUES
    (1, 1, 'Chaise', 'vintage', 'Vieille chaise venant des années 60 avec des griffures', 'under_estimation'),
    (2, 2, 'table', 'ghotique', 'Table manquant un pied', 'in_progress'),
    (3, 3, 'divan', 'moderne', 'Divan où le cuir est dégommé par des chiens', 'deposited');

    -- ======================
    -- ESTIMATIONS
    -- ======================
    INSERT INTO estimations (id, furniture_id, estimation_date, planned_work_description, planned_amount, validity_deadline, status)
    VALUES
    (1, 1, '2026-05-01', 'Ponçage and revernissage', 150.00, '2026-05-15', 'pending'),
    (2, 2, '2026-05-02', 'Remettre un pied', 220.00, '2026-05-20', 'accepted'),
    (3, 3, '2026-05-03', 'Remplacer le cuir et rembourrage', 400.00, '2026-05-25', 'accepted');

    -- ======================
    -- WORKSITES
    -- ======================
    INSERT INTO worksites (id, estimation_id, opening_date, final_amount, status)
    VALUES
    (1, 1, '2026-05-03', 160.00, 'pending'),
    (2, 2, '2026-05-04', NULL, 'open'),
    (3, 3, '2026-05-06', NULL, 'completed');

    -- ======================
    -- OPERATIONS
    -- ======================
    INSERT INTO operations (id, worksite_id, craftsman_registration, description, duration_minute, status)
    VALUES
    (1, 1, 'CR001', 'Ponçage du bois', 120, 'completed'),
    (2, 1, 'CR002', 'Application du vernis', 90, 'in_progress'),
    (3, 2, 'CR001', 'Remplacer le pied', 60, 'planned');

    -- ======================
    -- OPERATIONS_SUPPLIES
    -- ======================
    INSERT INTO operations_supplies (operation_id, supply_id, quantity_used)
    VALUES
    (1, 1, 2),
    (1, 4, 1),
    (2, 2, 1),
    (3, 1, 1),
    (3, 4, 3);

    COMMIT;
END $$

DELIMITER ;

CALL truncate_and_seed();

/*select * from supplies;


UPDATE estimations
set status = 'accepted'
where id = 1;

UPDATE worksites
set status = 'completed'
where id = 1;

select * from worksites;*/