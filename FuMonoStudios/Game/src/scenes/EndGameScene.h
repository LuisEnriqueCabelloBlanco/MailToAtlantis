#pragma once
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
private:
    void nextEnding();
    void loadEnd(Personaje npc, JSONObject& ent);
    ecs::Entity* endImage;
    ecs::Entity* endText;
    std::unordered_map<std::string,std::vector<std::string>> endTexts;
    int npcId;
};

