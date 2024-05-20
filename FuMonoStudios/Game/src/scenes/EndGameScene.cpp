#include <sdlutils/InputHandler.h>
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "EndGameScene.h"
#include <sistemas/ComonObjectsFactory.h>
#include <architecture/Game.h>
#include <entities/Final.h>
#include <cstdio>

EndGameScene::EndGameScene()
{
}

void EndGameScene::init()
{
    npcId_ = 0;
#ifdef _DEBUG
    std::cout << "init";
#endif // _DEBUG
    Personaje npc = (Personaje)npcId_;
    final_ = new Final(factory_, Vector2D(1600, 600), Vector2D(100, 200));
    final_->loadFinal(npc, gD().getNPCData(npc)->felicidad);

    remove( SAVE_PATH.c_str());

    // Fondo escena
    ComonObjectsFactory* fact = getFactory();
    fact->setLayer(ecs::layer::FOREGROUND);
    ecs::Entity* fondo = fact->createImage(Vector2D(0, 0), &sdlutils().images().at("finalFondo"));
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
            delete final_;
            final_ = nullptr;
            gm().requestChangeScene(ecs::sc::END_SCENE, ecs::sc::MENU_SCENE);
        }
    }
}

void EndGameScene::nextEnding()
{
    npcId_++;
    Personaje npc = (Personaje)npcId_;
    final_->loadFinal(npc, gD().getNPCData(npc)->felicidad);
}
