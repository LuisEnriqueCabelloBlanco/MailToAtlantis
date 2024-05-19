#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <architecture/Scene.h>


class FinalsInfoScene : public ecs::Scene
{
public:
    FinalsInfoScene();
    ~FinalsInfoScene() {};

    void init() override;
    virtual void update() override;
    void close() override;
private:
    void setActiveButtons(bool enable, std::vector<ecs::Entity*> buttons); // enable/disable npcButtons_ or enable/disable felicidadButtons_

    std::vector<ecs::Entity*> npcButtons_;
    std::vector<ecs::Entity*> felicidadButtons_; // will show the corresponding ending
};

