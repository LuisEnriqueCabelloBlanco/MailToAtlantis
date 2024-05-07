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
    Personaje npc = (Personaje)npcId_;
    final_ = new Final(this, npc, generalData().getNPCData(npc)->felicidad);
}

void EndGameScene::init()
{
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
    final_->loadFinal(npc, generalData().getNPCData(npc)->felicidad);
}
