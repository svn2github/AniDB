CREATE TABLE vtb (ver integer NOT NULL);
CREATE TABLE atb (
 aid integer NOT NULL,
 time timestamp NOT NULL default CURRENT_TIMESTAMP,
 romaji varchar(200) NOT NULL,
 kanji varchar(400) default NULL,
 english varchar(200) default NULL,
 year integer NOT NULL,
 episodes integer NOT NULL,
 last_ep integer NOT NULL,
 type varchar(16) NOT NULL,
 genre varchar(512) NOT NULL,
 img integer default 0,
 PRIMARY KEY (aid));
CREATE TABLE etb (
 eid integer NOT NULL,
 english varchar(200) NOT NULL,
 kanji varchar(400) default NULL,
 romaji varchar(200) default NULL,
 number varchar(16) NOT NULL,
 time timestamp NOT NULL default CURRENT_TIMESTAMP,
 PRIMARY KEY (eid));
CREATE TABLE gtb (
 gid integer NOT NULL,
 time timestamp NOT NULL default CURRENT_TIMESTAMP,
 name varchar(255) NOT NULL,
 short varchar(255) NOT NULL,
 PRIMARY KEY (gid));
CREATE TABLE ftb (
 fid integer NOT NULL,
 aid integer NOT NULL,
 eid integer NOT NULL,
 gid integer NOT NULL,
 state integer NOT NULL,
 size bigint NOT NULL,
 len integer NOT NULL DEFAULT 0,
 ed2k varchar(32) default NULL,
 md5 varchar(32) default NULL,
 sha1 varchar(40) default NULL,
 crc32 varchar(8) default NULL,
 ripsource varchar(16) default NULL,
 quality varchar(16) default NULL,
 audio varchar(255) default NULL,
 video varchar(16) default NULL,
 resolution varchar(16) default NULL,
 def_name varchar(255) NOT NULL,
 time timestamp NOT NULL default CURRENT_TIMESTAMP,
 sublang varchar(255) NOT NULL,
 dublang varchar(255) NOT NULL,
PRIMARY KEY (fid));
CREATE TABLE utb (
 uid serial,
 name varchar(10) NOT NULL,
 PRIMARY KEY (uid),
 UNIQUE (name));
CREATE TABLE dtb (
 did serial,
 name varchar(255) NOT NULL,
 PRIMARY KEY (did),
 UNIQUE (name));
CREATE TABLE jtb (
 orig varchar(255) NOT NULL,
 name varchar(255) NOT NULL,
 did integer NOT NULL,
 fid integer NOT NULL,
 status integer NOT NULL,
 ed2k varchar(32) NOT NULL,
 md5 varchar(32) default NULL,
 sha1 varchar(40) default NULL,
 tth varchar(39) default NULL,
 crc32 varchar(8) default NULL,
 size integer NOT NULL,
 uid integer NOT NULL,
 lid integer NOT NULL,
 time timestamp NOT NULL default CURRENT_TIMESTAMP,
 avxml TEXT DEFAULT NULL,
 PRIMARY KEY (size,ed2k),
 CONSTRAINT fk_did FOREIGN KEY (did) REFERENCES dtb (did),
 CONSTRAINT fk_fid FOREIGN KEY (fid) REFERENCES ftb (fid),
 CONSTRAINT fk_uid FOREIGN KEY (uid) REFERENCES utb (uid));
CREATE INDEX i_dtb_name ON dtb (name);
CREATE INDEX i_jtb_name ON jtb (name);
INSERT INTO ftb (fid,aid,eid,gid,state,size,ed2k,def_name,sublang,dublang) VALUES (0,0,0,0,0,0,'ed2k','','','');
INSERT INTO etb (eid,english,number) VALUES (0,'','');
INSERT INTO utb (name) VALUES ('default');
INSERT INTO vtb VALUES (4);