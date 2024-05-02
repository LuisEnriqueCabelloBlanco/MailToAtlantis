#include <utils/checkML.h>
#include "EndGameScene.h"
#include "../sistemas/ComonObjectsFactory.h"
#include <vector>
#include <json/JSON.h>
#include <architecture/GeneralData.h>
#include <string>
#include <sdlutils/InputHandler.h>
#include <components/Render.h>
#include <architecture/Game.h>
#include "entities/Finales.h"

EndGameScene::EndGameScene()
{
    npcId_ = 0;

}

void EndGameScene::init()
{
    endText_ = factory_->createLabel(Vector2D(300, 900), 1000,
        endTexts_[(Personaje)npcId_][generalData().getNPCData((Personaje)npcId_)->felicidad], 
        50,build_sdlcolor(0xFFFFFFFF));
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
    Finales::updateFinal(this, npc, generalData().getNPCData(npc)->felicidad);
}
