CREATE TABLE ext_packetlosshd_dbg (
  ip inet NOT NULL PRIMARY KEY,
  loss DOUBLE PRECISION NOT NULL DEFAULT 0,
  last_update TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT NOW(),
  last_read TIMESTAMP WITHOUT TIME ZONE NOT NULL DEFAULT NOW()
);

