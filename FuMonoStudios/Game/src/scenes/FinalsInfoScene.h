#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <architecture/Scene.h>
#include <architecture/GeneralData.h>

class Final;

class FinalsInfoScene : public ecs::Scene
{
/// <summary>
// Carga todos los elementos y botones una única vez solo al entrar al menu y luego tenemos métodos para
// Mostrar o desactivar esos botones
/// </summary>
public:
    FinalsInfoScene();
    ~FinalsInfoScene() {};

    void init() override;
    virtual void update() override;
    void close() override;
private:
    void setActiveButtons(bool enable, std::vector<ecs::Entity*> buttons); // enable/disable npcButtons_ o enable/disable felicidadButtons_

    std::vector<ecs::Entity*> npcButtons_;
    std::vector<ecs::Entity*> felicidadButtons_; // mostrara el final correspondiente


    // Usaremos estas dos variables para saber que final debe estar mostrandose
    Personaje selectedPer_; // lo actualizaran los npcButtons_
    Final* final_;
};

