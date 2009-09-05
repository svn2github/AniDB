ALTER TABLE job DROP INDEX FK_job_4, DROP FOREIGN KEY FK_job_4;
ALTER TABLE anime MODIFY COLUMN genre VARCHAR(512) CHARACTER SET ascii COLLATE ascii_general_ci NOT NULL DEFAULT '';
UPDATE anime SET english=NULL WHERE romaji=english OR english='';
UPDATE mylist SET state=1 WHERE lid=0;