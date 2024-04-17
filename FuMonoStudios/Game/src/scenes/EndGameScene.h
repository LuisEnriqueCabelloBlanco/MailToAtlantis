#pragma once
#include <architecture/Scene.h>
#include <json/JSON.h>
class EndGameScene :
    public ecs::Scene
{
public:
    EndGameScene();
    ~EndGameScene() {

    }
    void init() override;
private:

    void loadEnd(const std::string&, JSONObject& ent);
    ecs::Entity* endImage;
    std::unordered_map<std::string,std::vector<std::string>> endTexts;
};

