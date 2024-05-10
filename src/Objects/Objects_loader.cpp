/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "TestObjects.h"

void AddSC_omarion_60_4_B();

void AddObjects_Scripts()
{
    if (!(sConfigMgr->GetOption<bool>("TestObjects.Objects", false)))
        return;
    AddSC_omarion_60_4_B();
}
