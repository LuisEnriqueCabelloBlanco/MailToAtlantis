#pragma once
#include "PipeManager.h"
#include <sistemas/PipeManager.h>

using namespace tb;

class WorkRestrictionsSystem
{
public:
    WorkRestrictionsSystem();
    ~WorkRestrictionsSystem();
    void init();

    tb::WorkEvent getEvent(GeneralData::DialogSelection ds, int iteration);
private:
    std::string jsonPath;
};

