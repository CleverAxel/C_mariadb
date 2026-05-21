USE furniture_restoration;

-- --------------------
-- Création des rôles
-- --------------------
/*CREATE ROLE IF NOT EXISTS 'role_administrator'@'localhost';
CREATE ROLE IF NOT EXISTS 'role_secretary'@'localhost';
CREATE ROLE IF NOT EXISTS 'role_project_manager'@'localhost';*/

-- MariaDB syntax
CREATE ROLE IF NOT EXISTS 'role_administrator';
CREATE ROLE IF NOT EXISTS 'role_secretary';
CREATE ROLE IF NOT EXISTS 'role_project_manager';

-- -------------------------------
-- Ajout des privilèges aux rôles
-- -------------------------------

-- Rôle administrator
/*GRANT ALL PRIVILEGES
ON furniture_restoration.*
TO 'role_administrator'@'localhost';

-- Rôle secretary
GRANT SELECT, INSERT, UPDATE ON TABLE furniture_restoration.clients             TO 'role_secretary'@'localhost';
GRANT SELECT, INSERT, UPDATE ON TABLE furniture_restoration.furnitures          TO 'role_secretary'@'localhost';
GRANT SELECT, INSERT, UPDATE ON TABLE furniture_restoration.estimations         TO 'role_secretary'@'localhost';
GRANT SELECT, INSERT, UPDATE ON TABLE furniture_restoration.craftmans           TO 'role_secretary'@'localhost';
GRANT SELECT                 ON TABLE furniture_restoration.worksites           TO 'role_secretary'@'localhost';
GRANT SELECT                 ON TABLE furniture_restoration.supplies            TO 'role_secretary'@'localhost';
GRANT SELECT                 ON TABLE furniture_restoration.operations          TO 'role_secretary'@'localhost';
GRANT SELECT                 ON TABLE furniture_restoration.operations_supplies TO 'role_secretary'@'localhost';
GRANT SELECT                 ON TABLE furniture_restoration.logs                TO 'role_secretary'@'localhost';

-- Rôle project_manager
GRANT SELECT                 ON TABLE furniture_restoration.clients             TO 'role_project_manager'@'localhost';
GRANT SELECT                 ON TABLE furniture_restoration.furnitures          TO 'role_project_manager'@'localhost';
GRANT SELECT                 ON TABLE furniture_restoration.estimations         TO 'role_project_manager'@'localhost';
GRANT SELECT                 ON TABLE furniture_restoration.craftmans           TO 'role_project_manager'@'localhost';
GRANT SELECT, INSERT, UPDATE ON TABLE furniture_restoration.worksites           TO 'role_project_manager'@'localhost';
GRANT SELECT, INSERT, UPDATE ON TABLE furniture_restoration.supplies            TO 'role_project_manager'@'localhost';
GRANT SELECT, INSERT, UPDATE ON TABLE furniture_restoration.operations          TO 'role_project_manager'@'localhost';
GRANT SELECT, INSERT, UPDATE ON TABLE furniture_restoration.operations_supplies TO 'role_project_manager'@'localhost';
GRANT SELECT                 ON TABLE furniture_restoration.logs                TO 'role_project_manager'@'localhost';*/

-- Rôle administrator
GRANT ALL PRIVILEGES
ON furniture_restoration.*
TO role_administrator;

-- Rôle secretary
GRANT SELECT, INSERT, UPDATE ON furniture_restoration.clients             TO role_secretary;
GRANT SELECT, INSERT, UPDATE ON furniture_restoration.furnitures          TO role_secretary;
GRANT SELECT, INSERT, UPDATE ON furniture_restoration.estimations         TO role_secretary;
GRANT SELECT, INSERT, UPDATE ON furniture_restoration.craftmans           TO role_secretary;
GRANT SELECT                 ON furniture_restoration.worksites           TO role_secretary;
GRANT SELECT                 ON furniture_restoration.supplies            TO role_secretary;
GRANT SELECT                 ON furniture_restoration.operations          TO role_secretary;
GRANT SELECT                 ON furniture_restoration.operations_supplies TO role_secretary;
GRANT SELECT                 ON furniture_restoration.logs                TO role_secretary;

-- Rôle project_manager
GRANT SELECT                 ON furniture_restoration.clients             TO role_project_manager;
GRANT SELECT                 ON furniture_restoration.furnitures          TO role_project_manager;
GRANT SELECT                 ON furniture_restoration.estimations         TO role_project_manager;
GRANT SELECT                 ON furniture_restoration.craftmans           TO role_project_manager;
GRANT SELECT, INSERT, UPDATE ON furniture_restoration.worksites           TO role_project_manager;
GRANT SELECT, INSERT, UPDATE ON furniture_restoration.supplies            TO role_project_manager;
GRANT SELECT, INSERT, UPDATE ON furniture_restoration.operations          TO role_project_manager;
GRANT SELECT, INSERT, UPDATE ON furniture_restoration.operations_supplies TO role_project_manager;
GRANT SELECT                 ON furniture_restoration.logs                TO role_project_manager;

-- ---------------------------
-- Création des utilisateurs
-- ---------------------------
CREATE USER IF NOT EXISTS 'administrator'@'localhost'   IDENTIFIED BY 'password';
CREATE USER IF NOT EXISTS 'secretary'@'localhost'       IDENTIFIED BY 'password';
CREATE USER IF NOT EXISTS 'project_manager'@'localhost' IDENTIFIED BY 'password';

-- ----------------------------------------
-- Attribution des rôles aux utilisateurs
-- ----------------------------------------
/*GRANT 'role_administrator'@'localhost'   TO 'administrator'@'localhost';
GRANT 'role_secretary'@'localhost'       TO 'secretary'@'localhost';
GRANT 'role_project_manager'@'localhost' TO 'project_manager'@'localhost';*/

-- MariaDB syntax
GRANT role_administrator   TO 'administrator'@'localhost';
GRANT role_secretary       TO 'secretary'@'localhost';
GRANT role_project_manager TO 'project_manager'@'localhost';