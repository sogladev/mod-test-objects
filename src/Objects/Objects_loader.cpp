/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#include "TestObjects.h"

void AddSC_omarion_60_4_B();
void AddSC_test_nightbane();
void AddSC_test_hover();

void AddObjects_Scripts()
{
    if (!(sConfigMgr->GetOption<bool>("TestObjects.Objects", false)))
        return;
    AddSC_omarion_60_4_B();
    AddSC_test_nightbane();
    AddSC_test_hover();
}
