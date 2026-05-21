USE furniture_restoration;

DROP PROCEDURE IF EXISTS open_worksite;
DELIMITER $$
CREATE PROCEDURE open_worksite(IN p_worksite_id INT UNSIGNED)
BEGIN
    DECLARE _worksite_estimation_id INT UNSIGNED DEFAULT NULL;
    DECLARE _estimation_status VARCHAR(128) DEFAULT '';
    DECLARE _worksite_status VARCHAR(128) DEFAULT '';

    DECLARE _worksite_missing CONDITION FOR SQLSTATE '90001';
    DECLARE _worksite_already_open CONDITION FOR SQLSTATE '90002';
    DECLARE _estimation_wrong_status CONDITION FOR SQLSTATE '90002';

    SELECT w.estimation_id, e.status, w.status INTO _worksite_estimation_id, _estimation_status, _worksite_status
    FROM worksites AS w
    LEFT JOIN estimations AS e ON e.id = w.estimation_id
    WHERE w.id = p_worksite_id;

    IF _worksite_estimation_id IS NULL THEN
        SIGNAL _worksite_missing
            SET MESSAGE_TEXT = 'Le chantier n''existe pas';
    END IF;

    IF _estimation_status NOT IN ('accepted') THEN
        SIGNAL _estimation_wrong_status
            SET MESSAGE_TEXT = 'L''estimation du chantier doit être "accepté"';
    END IF;

    IF _worksite_status LIKE 'open' THEN
        SIGNAL _worksite_already_open
            SET MESSAGE_TEXT = 'Le chantier est déjà ouvert';
    END IF;
    
    UPDATE worksites
    SET status = 'open'
    WHERE id = p_worksite_id;

    SELECT TRUE AS 'worksite_opened';

END $$
DELIMITER ;


CALL open_worksite(1);
UPDATE estimations
SET planned_work_description = 'beutue'
WHERE id = 1;