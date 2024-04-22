#pragma once
#include "architecture/GeneralData.h"

class WorkRestrictionsSystem
{
    WorkRestrictionsSystem();
    ~WorkRestrictionsSystem();
    void init();

    void getRestrictionParams(GeneralData::DialogSelection ds, int iteration);
};

