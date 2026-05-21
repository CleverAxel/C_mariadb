DROP DATABASE IF EXISTS furniture_restoration;
CREATE DATABASE IF NOT EXISTS furniture_restoration
  CHARACTER SET utf8mb4;

USE furniture_restoration;

CREATE TABLE IF NOT EXISTS clients (
    id              INT UNSIGNED NOT NULL,

    lastname        VARCHAR(128) NOT NULL,
    firstname       VARCHAR(128) NOT NULL,
    address         VARCHAR(128) NOT NULL,
    phone           VARCHAR(64) NOT NULL,
    category        VARCHAR(50) NOT NULL,
    account_balance DECIMAL(10,2) NOT NULL DEFAULT 0.00,

    created_at      TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    created_by      VARCHAR(64) DEFAULT NULL,

    updated_at      TIMESTAMP DEFAULT NULL,
    updated_by      VARCHAR(64) DEFAULT NULL
)
ENGINE=InnoDB
CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS craftmans (
    registration_number VARCHAR(32),

    lastname            VARCHAR(128) NOT NULL,
    firstname           VARCHAR(128) NOT NULL,
    speciality          VARCHAR(128) NOT NULL,

    created_at          TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    created_by          VARCHAR(64) DEFAULT NULL,

    updated_at          TIMESTAMP DEFAULT NULL,
    updated_by          VARCHAR(64) DEFAULT NULL
)
ENGINE=InnoDB
CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS supplies (
    id                INT UNSIGNED NOT NULL,

    label             VARCHAR(128) NOT NULL,
    stock_quantity    INT UNSIGNED NOT NULL DEFAULT 0,
    minimum_threshold INT UNSIGNED NOT NULL DEFAULT 0,
    unit_cost         DECIMAL(10,2)  NOT NULL,
    status            ENUM('available','out_of_stock','discontinued') NOT NULL DEFAULT 'available',

    created_at        TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    created_by        VARCHAR(64) DEFAULT NULL,

    updated_at        TIMESTAMP DEFAULT NULL,
    updated_by        VARCHAR(64) DEFAULT NULL
)
ENGINE=InnoDB
CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS furnitures (
    id             INT UNSIGNED,
    client_id      INT UNSIGNED NOT NULL,

    furniture_type VARCHAR(100) NOT NULL,
    style          VARCHAR(100) NOT NULL,
    description    TEXT,
    deposit_date   TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    status         ENUM('deposited','under_estimation','in_progress','completed','returned') NOT NULL DEFAULT 'deposited',

    created_at     TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    created_by     VARCHAR(64) DEFAULT NULL,

    updated_at     TIMESTAMP DEFAULT NULL,
    updated_by     VARCHAR(64) DEFAULT NULL
)
ENGINE=InnoDB
CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS estimations (
    id                        INT UNSIGNED,
    furniture_id              INT UNSIGNED NOT NULL,

    estimation_date           DATE NOT NULL,
    planned_work_description  TEXT,
    planned_amount            DECIMAL(10,2) NOT NULL,
    validity_deadline         DATE NOT NULL,
    status                    ENUM('pending','accepted','refused','expired') NOT NULL DEFAULT 'pending',

    created_at                TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    created_by                VARCHAR(64) DEFAULT NULL,

    updated_at                TIMESTAMP DEFAULT NULL,
    updated_by                VARCHAR(64) DEFAULT NULL
)
ENGINE=InnoDB
CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS worksites (
    id              INT UNSIGNED,
    estimation_id   INT UNSIGNED NOT NULL,

    opening_date    DATE NOT NULL,
    final_amount    DECIMAL(10,2),
    status          ENUM('open','in_progress','completed','cancelled', 'pending') NOT NULL DEFAULT 'pending',

    created_at      TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    created_by      VARCHAR(64) DEFAULT NULL,
    
    updated_at      TIMESTAMP DEFAULT NULL,
    updated_by      VARCHAR(64) DEFAULT NULL
)
ENGINE=InnoDB
CHARSET=utf8mb4;

CREATE TABLE operations (
    id                     INT UNSIGNED,
    worksite_id            INT UNSIGNED NOT NULL,
    craftsman_registration VARCHAR(32) NOT NULL,

    description            TEXT NOT NULL,
    duration_minute        INT UNSIGNED NOT NULL DEFAULT 0,
    status                 ENUM('planned','in_progress','completed','cancelled') NOT NULL DEFAULT 'planned',

    created_at             TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    created_by             VARCHAR(64) DEFAULT NULL,
        
    updated_at             TIMESTAMP DEFAULT NULL,
    updated_by             VARCHAR(64) DEFAULT NULL
)
ENGINE=InnoDB
CHARSET=utf8mb4;

CREATE TABLE operations_supplies (
    operation_id   INT UNSIGNED NOT NULL,
    supply_id      INT UNSIGNED NOT NULL,
    quantity_used  INT UNSIGNED NOT NULL DEFAULT 1
)
ENGINE=InnoDB
CHARSET=utf8mb4;

CREATE TABLE IF NOT EXISTS logs (
id INT UNSIGNED,
table_name VARCHAR(64) NOT NULL,
moment_type ENUM('BEFORE', 'AFTER') NOT NULL,
action_type ENUM('INSERT', 'UPDATE', 'DELETE') NOT NULL,
primary_key VARCHAR(255),
created_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
created_by VARCHAR(64) NOT NULL
)
ENGINE=InnoDB
CHARSET=utf8mb4;

-- ------------------------------------------------------------------

ALTER TABLE logs
ADD CONSTRAINT pk_log PRIMARY KEY (id);

ALTER TABLE clients
ADD CONSTRAINT pk_client PRIMARY KEY (id);

ALTER TABLE craftmans
ADD CONSTRAINT pk_craftmans PRIMARY KEY (registration_number);

ALTER TABLE supplies
ADD CONSTRAINT pk_supply PRIMARY KEY (id);

ALTER TABLE furnitures
ADD CONSTRAINT pk_furniture PRIMARY KEY (id);

ALTER TABLE estimations
ADD CONSTRAINT pk_estimation PRIMARY KEY (id);

ALTER TABLE worksites
ADD CONSTRAINT pk_worksite PRIMARY KEY (id);

ALTER TABLE operations
ADD CONSTRAINT pk_operation PRIMARY KEY (id);

ALTER TABLE operations_supplies
ADD CONSTRAINT pk_operation_supply PRIMARY KEY (operation_id, supply_id);

-- ------------------------------------------------------------------

ALTER TABLE logs
MODIFY COLUMN id INT UNSIGNED AUTO_INCREMENT;

ALTER TABLE clients
MODIFY COLUMN id INT UNSIGNED AUTO_INCREMENT;

ALTER TABLE supplies
MODIFY COLUMN id INT UNSIGNED AUTO_INCREMENT;

ALTER TABLE furnitures
MODIFY COLUMN id INT UNSIGNED AUTO_INCREMENT;

ALTER TABLE estimations
MODIFY COLUMN id INT UNSIGNED AUTO_INCREMENT;

ALTER TABLE worksites
MODIFY COLUMN id INT UNSIGNED AUTO_INCREMENT;

ALTER TABLE operations
MODIFY COLUMN id INT UNSIGNED AUTO_INCREMENT;

-- ------------------------------------------------------------------

ALTER TABLE furnitures
ADD CONSTRAINT fk_furnitures_client
FOREIGN KEY (client_id)
REFERENCES clients(id)
ON DELETE RESTRICT
ON UPDATE RESTRICT;

ALTER TABLE estimations
ADD CONSTRAINT fk_estimations_furniture
FOREIGN KEY (furniture_id)
REFERENCES furnitures(id)
ON DELETE RESTRICT
ON UPDATE RESTRICT;

ALTER TABLE worksites
ADD CONSTRAINT fk_worksites_estimation
FOREIGN KEY (estimation_id)
REFERENCES estimations(id)
ON DELETE RESTRICT
ON UPDATE RESTRICT;

ALTER TABLE operations
ADD CONSTRAINT fk_operations_worksite
FOREIGN KEY (worksite_id)
REFERENCES worksites(id)
ON DELETE RESTRICT
ON UPDATE RESTRICT;

ALTER TABLE operations
ADD CONSTRAINT fk_operations_craftsman
FOREIGN KEY (craftsman_registration)
REFERENCES craftmans(registration_number)
ON DELETE RESTRICT
ON UPDATE RESTRICT;

ALTER TABLE operations_supplies
ADD CONSTRAINT fk_opsup_operation
FOREIGN KEY (operation_id)
REFERENCES operations(id)
ON DELETE RESTRICT
ON UPDATE RESTRICT;

ALTER TABLE operations_supplies
ADD CONSTRAINT fk_opsup_supply
FOREIGN KEY (supply_id)
REFERENCES supplies(id)
ON DELETE RESTRICT
ON UPDATE RESTRICT;