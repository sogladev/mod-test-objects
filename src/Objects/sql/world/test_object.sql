--
-- Tarsis
-- Omarion
DELETE FROM `creature` WHERE `guid` BETWEEN 380000 AND 381000;
INSERT INTO `creature` (`guid`, `id1`, `id2`, `id3`, `map`, `spawnMask`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `MovementType`) VALUES
(380000, 16365, 0, 0, 573, 0, 0, 1, 1, 0, 1908.97, 638.718, 0.000182854, 0.0999733, 300, 0.0, 0, 4979, 0, 0, 0, 0, 0, '', NULL, 0, NULL),
(380001, 16381, 0, 0, 573, 0, 0, 1, 1, 0, 1908.48, 634.065, 0.000182854, 6.24218, 300, 0.0, 0, 256410, 243400, 0, 0, 0, 0, '', NULL, 0, NULL);

-- .tele exteriortest
INSERT INTO `game_tele` (`position_x`, `position_y`, `position_z`, `orientation`, `map`, `name`)
VALUES(1931.47, 630.79, 0.0005, 1, 573, 'exteriortest');
