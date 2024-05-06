#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
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
    void loadEnd(Personaje npc, JSONObject& ent);
    ecs::Entity* endImage_;
    ecs::Entity* endText_;
    NPCdata* currentNPC;
    std::unordered_map<Personaje,std::unordered_map<Felicidad,std::string>> endTexts_;
    int npcId_;
};

