#pragma once
#include <utils/checkML.h>
#include <architecture/Scene.h>
#include <json/JSON.h>
#include <architecture/GeneralData.h>
class EndGameScene :
    public ecs::Scene
{
public:
    EndGameScene();
    ~EndGameScene() {

    }
    void init() override;
    virtual void update() override;
private:
    void nextEnding();
    ecs::Entity* endImage_;
    ecs::Entity* endText_;
    NPCdata* currentNPC;
    int npcId_;
    double minTime = 1000; // Tiempo minimo antes de poder pasar al siguiente final, en milesimas
    VirtualTimer timer_;
};

