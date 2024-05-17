#include <sdlutils/InputHandler.h>
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "EndGameScene.h"
#include <sistemas/ComonObjectsFactory.h>
#include <vector>
#include <json/JSON.h>
#include <architecture/GeneralData.h>
#include <string>
#include <components/Render.h>
#include <architecture/Game.h>
#include "entities/Final.h"

EndGameScene::EndGameScene()
{
    npcId_ = 0;
}

void EndGameScene::init()
{
    Personaje npc = (Personaje)npcId_;
    final_ = new Final(this, npc, GeneralData().getNPCData(npc)->felicidad);

    // Fondo escena
    ComonObjectsFactory* fact = getFactory();
    fact->setLayer(ecs::layer::FOREGROUND);
    Texture* fondoTex = &sdlutils().images().at("finalFondo");
    ecs::Entity* fondo = fact->createImage(Vector2D(0, 0), Vector2D(fondoTex->width(), fondoTex->height()), fondoTex);
    Transform* fondoTr = fondo->getComponent<Transform>();
    fondoTr->setScale(1);
}

void EndGameScene::update()
{
    // Primero comprobamos si jugador pulsa y cooldown
    if (ih().mouseButtonDownEvent() && timer_.currTime() > minTime) {
        timer_.reset();
        if (npcId_ < 6) {
            nextEnding();
        }
        else {
            gm().requestChangeScene(ecs::sc::END_SCENE, ecs::sc::MENU_SCENE);
        }
    }
}

void EndGameScene::nextEnding()
{
    npcId_++;
    Personaje npc = (Personaje)npcId_;
    final_->loadFinal(npc, GeneralData().getNPCData(npc)->felicidad);
}
