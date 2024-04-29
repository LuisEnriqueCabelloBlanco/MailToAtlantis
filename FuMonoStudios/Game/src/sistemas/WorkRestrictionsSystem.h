#pragma once
#include <sistemas/PipeManager.h>
#include <json/JSON.h>


using namespace tb;

class WorkRestrictionsSystem
{
public:
    WorkRestrictionsSystem();
    ~WorkRestrictionsSystem();
    void init();

    tb::WorkEvent getRandomEvent();
    tb::WorkEvent getEvent(int selection);
private:
    std::string jsonPath;
    void eventSwapPipe(WorkEvent& event, JSONObject jsonObj);
    void eventBanType(WorkEvent& event, JSONObject jsonObj);
    void eventWeightRes(WorkEvent& event, JSONObject jsonObj);
};

