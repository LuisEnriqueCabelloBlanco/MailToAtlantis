#pragma once
#include <architecture/Scene.h>
class EndGameScene :
    public ecs::Scene
{
public:
    EndGameScene(){

    }
    ~EndGameScene() {

    }
    void init() override;
private:
    ecs::Entity* endImage;
};

