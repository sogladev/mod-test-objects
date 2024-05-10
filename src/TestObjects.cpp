/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "TestObjects.h"
#include "DBUpdater.h"

inline std::vector<std::string> GetDatabaseDirectories(std::string const& folderName)
{
    std::vector<std::string> directories;

    std::string const path = "/modules/mod-test-objects/src/";
    for (std::string const& bracketName : TestObjectNames)
    {
        if (!(sConfigMgr->GetOption<bool>("TestObjects." + bracketName, false)))
        {
            continue;
        }

        std::string bracketPath = path + bracketName + "/sql/" + folderName;
        directories.push_back(std::move(bracketPath));
    }

    return directories;
}

class TestObjectsLoadingDBUpdates : public DatabaseScript
{
public:
    TestObjectsLoadingDBUpdates() : DatabaseScript("TestObjectsLoadingDBUpdates") {}

    void OnAfterDatabasesLoaded(uint32 updateFlags) override
    {
        LOG_INFO("server.server", "Loading mod-test-objects updates...");

        if (DBUpdater<LoginDatabaseConnection>::IsEnabled(updateFlags))
        {
            if (sConfigMgr->GetOption<bool>("TestObjects.ReapplyUpdates", true))
            {
                LoginDatabase.Query("DELETE FROM updates WHERE name LIKE '%test_object%'");
            }

            std::vector<std::string> loginDatabaseDirectories = GetDatabaseDirectories("auth");
            if (!loginDatabaseDirectories.empty())
            {
                DBUpdater<LoginDatabaseConnection>::Update(LoginDatabase, &loginDatabaseDirectories);
            }
        }

        if (DBUpdater<CharacterDatabaseConnection>::IsEnabled(updateFlags))
        {
            if (sConfigMgr->GetOption<bool>("TestObjects.ReapplyUpdates", true))
            {
                CharacterDatabase.Query("DELETE FROM updates WHERE name LIKE '%test_object%'");
            }

            std::vector<std::string> charactersDatabaseDirectories = GetDatabaseDirectories("characters");
            if (!charactersDatabaseDirectories.empty())
            {
                DBUpdater<CharacterDatabaseConnection>::Update(CharacterDatabase, &charactersDatabaseDirectories);
            }
        }

        if (DBUpdater<WorldDatabaseConnection>::IsEnabled(updateFlags))
        {
            if (sConfigMgr->GetOption<bool>("TestObjects.ReapplyUpdates", true))
            {
                WorldDatabase.Query("DELETE FROM updates WHERE name LIKE '%test_object%'");
            }

            std::vector<std::string> worldDatabaseDirectories = GetDatabaseDirectories("world");
            if (!worldDatabaseDirectories.empty())
            {
                DBUpdater<WorldDatabaseConnection>::Update(WorldDatabase, &worldDatabaseDirectories);
            }
        }
    }
};

TestObjects* TestObjects::instance()
{
    static TestObjects instance;
    return &instance;
}

class TestObjects_WorldScript : public WorldScript
{
private:
    static void LoadConfig()
    {
        sTestObjects->enabled = sConfigMgr->GetOption<bool>("TestObjects.Enable", true);
        // sTestObjects->requireAttunement = sConfigMgr->GetOption<bool>("TestObjects.Objects", true);
    }

public:
    TestObjects_WorldScript() : WorldScript("TestObjects_WorldScript") { }

    void OnBeforeConfigLoad(bool /*reload*/) override
    {
        LoadConfig();
    }
};

void AddTestObjectsScripts()
{
    if (sConfigMgr->GetOption<bool>("TestObjects.LoadDatabase", true))
    {
        new TestObjectsLoadingDBUpdates();
    }
    new TestObjects_WorldScript();
}
