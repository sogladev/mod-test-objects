/*
 * Copyright (C) 2016+ AzerothCore <www.azerothcore.org>, released under GNU AGPL v3 license: https://github.com/azerothcore/azerothcore-wotlk/blob/master/LICENSE-AGPL3
 */

#ifndef _TEST_OBJECTS_H_
#define _TEST_OBJECTS_H_

#include "Config.h"
#include "ScriptMgr.h"

#define OBJECTS_MAX 1
std::array<std::string, OBJECTS_MAX> const TestObjectNames =
{
    "Objects",
};

class TestObjects
{
public:
    static TestObjects* instance();

    bool enabled;
};

#define sTestObjects TestObjects::instance()

#endif // _TEST_OBJECTS_H_
