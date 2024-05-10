--
-- Test Nightbane
-- ScriptName: test_hover
SET @ENTRY := 385001;
SET @CGUID := 380004;

SET @POSITION_X := -11127.625;
SET @POSITION_Y := -2043.02;
SET @POSITION_Z := 47.075;
SET @POSITION_O := 1.4655;
SET @MAP_ID := 532;

-- Spawn
DELETE FROM `creature` WHERE (`id1` = @ENTRY);
INSERT INTO `creature` (`guid`, `id1`, `id2`, `id3`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `wander_distance`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`, `ScriptName`, `VerifiedBuild`, `CreateObject`, `Comment`) VALUES
(@CGUID, @ENTRY, 0, 0, @MAP_ID, 0, 0, 1, 1, 0, @POSITION_X, @POSITION_Y, @POSITION_Z, @POSITION_O, 604800, 0, 0, 928550, 0, 0, 0, 0, 0, '', 0, 0, NULL);

DELETE FROM `creature_addon` WHERE (`guid` IN (@CGUID));
INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `visibilityDistanceType`, `auras`) VALUES
(@CGUID, 0, 0, 3, 1, 0, 0, NULL);
-- Template
DELETE FROM `creature_template` WHERE (`entry` = @ENTRY);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction`, `npcflag`, `speed_walk`, `speed_run`, `speed_swim`, `speed_flight`, `detection_range`, `scale`, `rank`, `dmgschool`, `DamageModifier`, `BaseAttackTime`, `RangeAttackTime`, `BaseVariance`, `RangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `HoverHeight`, `HealthModifier`, `ManaModifier`, `ArmorModifier`, `ExperienceModifier`, `RacialLeader`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `spell_school_immune_mask`, `flags_extra`, `ScriptName`, `VerifiedBuild`) VALUES
(@ENTRY, 0, 0, 0, 0, 0, 18062, 0, 0, 0, 'Hover Test', NULL, NULL, 0, 73, 73, 1, 103, 0, 5.6, 4, 1, 1, 20, 1, 3, 0, 32, 2000, 2000, 1, 1, 1, 256, 2048, 0, 0, 0, 0, 0, 0, 6, 108, @ENTRY, 0, 0, 0, 0, 110755, 144638, '', 0, 1, 122.5, 20, 1, 1, 0, 255, 1, 650854235, 0, 259, 'test_hover', 12340);
DELETE FROM `creature_template_addon` WHERE (`entry` = @ENTRY);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `visibilityDistanceType`, `auras`) VALUES
(@ENTRY, 0, 0, 3, 1, 0, 3, NULL);
DELETE FROM `creature_template_movement` WHERE (`CreatureId` = @ENTRY);
INSERT INTO `creature_template_movement` (`CreatureId`, `Ground`, `Swim`, `Flight`, `Rooted`, `Chase`, `Random`, `InteractionPauseTimer`) VALUES
(@ENTRY, 1, 1, 2, 0, 0, 0, NULL);
DELETE FROM `creature_onkill_reputation` WHERE (`creature_id` = @ENTRY);
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `IsTeamAward1`, `RewOnKillRepValue1`, `MaxStanding2`, `IsTeamAward2`, `RewOnKillRepValue2`, `TeamDependent`) VALUES
(@ENTRY, 967, 0, 7, 0, 250, 0, 0, 0, 0);
DELETE FROM `creature_questitem` WHERE (`CreatureEntry` = @ENTRY);
INSERT INTO `creature_questitem` (`CreatureEntry`, `Idx`, `ItemId`, `VerifiedBuild`) VALUES
(@ENTRY, 0, 31751, 0),
(@ENTRY, 1, 24139, 0);
DELETE FROM `creature_loot_template` WHERE (`Entry` = @ENTRY);
INSERT INTO `creature_loot_template` (`Entry`, `Item`, `Reference`, `Chance`, `QuestRequired`, `LootMode`, `GroupId`, `MinCount`, `MaxCount`, `Comment`) VALUES
(@ENTRY, 22903, 0, 0.2, 0, 1, 0, 1, 1, 'Nightbane - Recipe: Insane Strength Potion'),
(@ENTRY, 23884, 0, 0.2, 0, 1, 0, 1, 1, 'Nightbane - Schematic: Mana Potion Injector'),
(@ENTRY, 24139, 0, 100, 1, 1, 0, 1, 1, 'Nightbane - Faint Arcane Essence'),
(@ENTRY, 29434, 0, 100, 0, 1, 0, 2, 2, 'Nightbane - Badge of Justice'),
(@ENTRY, 31751, 0, 100, 1, 1, 0, 1, 1, 'Nightbane - Blazing Signet'),
(@ENTRY, 34022, 34022, 100, 0, 1, 1, 1, 2, 'Nightbane - (ReferenceTable)');
