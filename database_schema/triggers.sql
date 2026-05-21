USE furniture_restoration;

-- BEFORE TRIGGERS - will set the current user to the table being modified
DROP TRIGGER IF EXISTS trigger_before_insert_clients;
DELIMITER $$
CREATE TRIGGER trigger_before_insert_clients
BEFORE INSERT ON clients
FOR EACH ROW
BEGIN
    SET NEW.created_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'BEFORE', NULL, 'clients', CURRENT_USER());
END $$
DELIMITER ;


DROP TRIGGER IF EXISTS trigger_before_update_clients;
DELIMITER $$
CREATE TRIGGER trigger_before_update_clients
BEFORE UPDATE ON clients
FOR EACH ROW
BEGIN
    SET NEW.updated_at = CURRENT_TIMESTAMP;
    SET NEW.updated_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'BEFORE', NULL, 'clients', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_before_insert_craftmans;
DELIMITER $$
CREATE TRIGGER trigger_before_insert_craftmans
BEFORE INSERT ON craftmans
FOR EACH ROW
BEGIN
    SET NEW.created_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'BEFORE', NULL, 'craftmans', CURRENT_USER());
END $$
DELIMITER ;
DROP TRIGGER IF EXISTS trigger_before_update_craftmans;
DELIMITER $$
CREATE TRIGGER trigger_before_update_craftmans
BEFORE UPDATE ON craftmans
FOR EACH ROW
BEGIN
    SET NEW.updated_at = CURRENT_TIMESTAMP;
    SET NEW.updated_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'BEFORE', NULL, 'craftmans', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_before_insert_supplies;
DELIMITER $$
CREATE TRIGGER trigger_before_insert_supplies
BEFORE INSERT ON supplies
FOR EACH ROW
BEGIN
    SET NEW.created_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'BEFORE', NULL, 'supplies', CURRENT_USER());
END $$
DELIMITER ;


DROP TRIGGER IF EXISTS trigger_before_update_supplies;
DELIMITER $$
CREATE TRIGGER trigger_before_update_supplies
BEFORE UPDATE ON supplies
FOR EACH ROW
BEGIN

    -- Solution de facilité pour simuler un call de réapprovisonnement
    IF NEW.stock_quantity <= NEW.minimum_threshold THEN
        SET NEW.stock_quantity = NEW.stock_quantity + NEW.minimum_threshold * 2;
        SET NEW.status = 'available';
    END IF;

    SET NEW.updated_at = CURRENT_TIMESTAMP;
    SET NEW.updated_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'BEFORE', NULL, 'supplies', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_before_insert_furnitures;
DELIMITER $$
CREATE TRIGGER trigger_before_insert_furnitures
BEFORE INSERT ON furnitures
FOR EACH ROW
BEGIN
    SET NEW.created_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'BEFORE', NULL, 'furnitures', CURRENT_USER());
END $$
DELIMITER ;


DROP TRIGGER IF EXISTS trigger_before_update_furnitures;
DELIMITER $$
CREATE TRIGGER trigger_before_update_furnitures
BEFORE UPDATE ON furnitures
FOR EACH ROW
BEGIN
    SET NEW.updated_at = CURRENT_TIMESTAMP;
    SET NEW.updated_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'BEFORE', NULL, 'furnitures', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_before_insert_estimations;
DELIMITER $$
CREATE TRIGGER trigger_before_insert_estimations
BEFORE INSERT ON estimations
FOR EACH ROW
BEGIN
    SET NEW.created_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'BEFORE', NULL, 'estimations', CURRENT_USER());
END $$
DELIMITER ;


DROP TRIGGER IF EXISTS trigger_before_update_estimations;
DELIMITER $$
CREATE TRIGGER trigger_before_update_estimations
BEFORE UPDATE ON estimations
FOR EACH ROW
BEGIN
    DECLARE _worksite_status VARCHAR(50) DEFAULT NULL;
    DECLARE _unable_update_estimation_worksite_opened FOR SQLSTATE '90001';
    DECLARE _unable_update_estimation_worksite_completed FOR SQLSTATE '90002';

    SELECT w.status INTO _worksite_status
    FROM worksites w
    WHERE w.estimation_id = NEW.id;

    IF _worksite_status = 'open' THEN
        SIGNAL _unable_update_estimation_worksite_opened
            SET MESSAGE_TEXT = 'Impossible de mettre à jour l''estimation, le chantier lui étant associé est ouvert.';
    ELSEIF _worksite_status = 'completed' THEN
        SIGNAL _unable_update_estimation_worksite_completed
            SET MESSAGE_TEXT = 'Impossible de mettre à jour l''estimation, le chantier lui étant associé est complété.';
    END IF;



    SET NEW.updated_at = CURRENT_TIMESTAMP;
    SET NEW.updated_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'BEFORE', OLD.id, 'estimations', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_before_insert_worksites;
DELIMITER $$
CREATE TRIGGER trigger_before_insert_worksites
BEFORE INSERT ON worksites
FOR EACH ROW
BEGIN
    DECLARE _estimation_status VARCHAR(128) DEFAULT '';
    DECLARE _unable_open_worksite FOR SQLSTATE '90003';

    IF NEW.status = 'open' THEN
        SELECT status INTO _estimation_status
        FROM estimations
        WHERE id = NEW.estimation_id;

        IF _estimation_status NOT IN ('accepted') THEN
            SIGNAL _unable_open_worksite
                SET MESSAGE_TEXT = 'L''estimation du chantier doit être "accepté", pour pouvoir l''ouvrir';
        END IF;
    END IF;

    SET NEW.created_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'BEFORE', NULL, 'worksites', CURRENT_USER());
END $$
DELIMITER ;


DROP TRIGGER IF EXISTS trigger_before_update_worksites;
DELIMITER $$
CREATE TRIGGER trigger_before_update_worksites
BEFORE UPDATE ON worksites
FOR EACH ROW
BEGIN
    DECLARE _estimation_status VARCHAR(128) DEFAULT ''; 
    DECLARE _unable_open_worksite FOR SQLSTATE '90003';
    DECLARE _unable_update_worksite_completed FOR SQLSTATE '90004';

    IF NEW.status = 'open' THEN
        SELECT status INTO _estimation_status
        FROM estimations
        WHERE id = NEW.estimation_id;

        IF _estimation_status NOT IN ('accepted') THEN
            SIGNAL _unable_open_worksite
                SET MESSAGE_TEXT = 'L''estimation du chantier doit être "accepté", pour pouvoir l''ouvrir';
        END IF;
    END IF;

    IF OLD.status = 'completed' THEN
        SIGNAL _unable_update_worksite_completed
            SET MESSAGE_TEXT = 'Impossible de mettre à jour le chantier, celui-ci est terminé';
    END IF;

    SET NEW.updated_at = CURRENT_TIMESTAMP;
    SET NEW.updated_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'BEFORE', NULL, 'worksites', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_before_insert_operations;
DELIMITER $$
CREATE TRIGGER trigger_before_insert_operations
BEFORE INSERT ON operations
FOR EACH ROW
BEGIN
    DECLARE _worksite_status VARCHAR(64) DEFAULT NULL;
    DECLARE _unable_update_worksite_completed FOR SQLSTATE '90004';

    SELECT w.status INTO _worksite_status
    FROM worksites w
    WHERE w.id = NEW.worksite_id;

    IF _worksite_status = 'completed' THEN
        SIGNAL _unable_update_worksite_completed
            SET MESSAGE_TEXT = 'Impossible d''ajouter une opération, le chantier est terminé';
    END IF;

    SET NEW.created_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'BEFORE', NULL, 'operations', CURRENT_USER());
END $$
DELIMITER ;


DROP TRIGGER IF EXISTS trigger_before_update_operations;
DELIMITER $$
CREATE TRIGGER trigger_before_update_operations
BEFORE UPDATE ON operations
FOR EACH ROW
BEGIN
    DECLARE _unable_update_operation_completed FOR SQLSTATE '90005';

    IF OLD.status = 'completed' THEN
        SIGNAL _unable_update_operation_completed
            SET MESSAGE_TEXT = 'Impossible de mettre à jour l''opération, celle-ci est terminée';
    END IF;

    SET NEW.updated_at = CURRENT_TIMESTAMP;
    SET NEW.updated_by = CURRENT_USER();
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'BEFORE', NULL, 'operations', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_before_insert_operations_supplies;
DELIMITER $$
CREATE TRIGGER trigger_before_insert_operations_supplies
BEFORE INSERT ON operations_supplies
FOR EACH ROW
BEGIN
    DECLARE _available_stock INT UNSIGNED DEFAULT NULL;

    SELECT stock_quantity INTO _available_stock
    FROM supplies
    WHERE id = NEW.supply_id;

    IF _available_stock < NEW.quantity_used THEN
        UPDATE supplies
        SET stock_quantity = 0
        WHERE id = NEW.supply_id;

        SET NEW.quantity_used = _available_stock;
    ELSE
        UPDATE supplies
        SET stock_quantity = stock_quantity - NEW.quantity_used
        WHERE id = NEW.supply_id;
    END IF;

    
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'BEFORE', NULL, 'operations_supplies', CURRENT_USER());
END $$
DELIMITER ;



-- AFTER triggers 
DROP TRIGGER IF EXISTS trigger_after_insert_clients;
DELIMITER $$
CREATE TRIGGER trigger_after_insert_clients
AFTER INSERT ON clients
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'AFTER', NEW.id, 'clients', CURRENT_USER());
END $$
DELIMITER ;
DROP TRIGGER IF EXISTS trigger_after_update_clients;
DELIMITER $$
CREATE TRIGGER trigger_after_update_clients
AFTER UPDATE ON clients
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'AFTER', NEW.id, 'clients', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_after_insert_craftmans;
DELIMITER $$
CREATE TRIGGER trigger_after_insert_craftmans
AFTER INSERT ON craftmans
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'AFTER', NEW.registration_number, 'craftmans', CURRENT_USER());
END $$
DELIMITER ;


DROP TRIGGER IF EXISTS trigger_after_update_craftmans;
DELIMITER $$
CREATE TRIGGER trigger_after_update_craftmans
AFTER UPDATE ON craftmans
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'AFTER', NEW.registration_number, 'craftmans', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_after_insert_supplies;
DELIMITER $$
CREATE TRIGGER trigger_after_insert_supplies
AFTER INSERT ON supplies
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'AFTER', NEW.id, 'supplies', CURRENT_USER());
END $$
DELIMITER ;


DROP TRIGGER IF EXISTS trigger_after_update_supplies;
DELIMITER $$
CREATE TRIGGER trigger_after_update_supplies
AFTER UPDATE ON supplies
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'AFTER', NEW.id, 'supplies', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_after_insert_furnitures;
DELIMITER $$
CREATE TRIGGER trigger_after_insert_furnitures
AFTER INSERT ON furnitures
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'AFTER', NEW.id, 'furnitures', CURRENT_USER());
END $$
DELIMITER ;


DROP TRIGGER IF EXISTS trigger_after_update_furnitures;
DELIMITER $$
CREATE TRIGGER trigger_after_update_furnitures
AFTER UPDATE ON furnitures
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'AFTER', NEW.id, 'furnitures', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_after_insert_estimations;
DELIMITER $$
CREATE TRIGGER trigger_after_insert_estimations
AFTER INSERT ON estimations
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'AFTER', NEW.id, 'estimations', CURRENT_USER());
END $$
DELIMITER ;


DROP TRIGGER IF EXISTS trigger_after_update_estimations;
DELIMITER $$
CREATE TRIGGER trigger_after_update_estimations
AFTER UPDATE ON estimations
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'AFTER', NEW.id, 'estimations', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_after_insert_worksites;
DELIMITER $$
CREATE TRIGGER trigger_after_insert_worksites
AFTER INSERT ON worksites
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'AFTER', NEW.id, 'worksites', CURRENT_USER());
END $$
DELIMITER ;


DROP TRIGGER IF EXISTS trigger_after_update_worksites;
DELIMITER $$
CREATE TRIGGER trigger_after_update_worksites
AFTER UPDATE ON worksites
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'AFTER', NEW.id, 'worksites', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_after_insert_operations;
DELIMITER $$
CREATE TRIGGER trigger_after_insert_operations
AFTER INSERT ON operations
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'AFTER', NEW.id, 'operations', CURRENT_USER());
END $$
DELIMITER ;


DROP TRIGGER IF EXISTS trigger_after_update_operations;
DELIMITER $$
CREATE TRIGGER trigger_after_update_operations
AFTER UPDATE ON operations
FOR EACH ROW
BEGIN
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('UPDATE', 'AFTER', NEW.id, 'operations', CURRENT_USER());
END $$
DELIMITER ;

-- ------------------------------------------------------------------

DROP TRIGGER IF EXISTS trigger_after_insert_operations_supplies;
DELIMITER $$
CREATE TRIGGER trigger_after_insert_operations_supplies
AFTER INSERT ON operations_supplies
FOR EACH ROW
BEGIN    
    INSERT INTO logs(action_type, moment_type, primary_key, table_name, created_by)
    VALUES ('INSERT', 'AFTER', NULL, 'operations_supplies', CURRENT_USER());
END $$
DELIMITER ;