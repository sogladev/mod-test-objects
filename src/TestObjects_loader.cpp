/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "Config.h"

void AddTestObjectsScripts();
void AddObjects_Scripts();

void Addmod_test_objectsScripts()
{
    AddTestObjectsScripts();

    if (!sConfigMgr->GetOption<bool>("TestObjects.LoadScripts", true))
        return;

    AddObjects_Scripts();
}
