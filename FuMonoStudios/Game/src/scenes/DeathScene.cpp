#include "DeathScene.h"
#include <utils/checkML.h>
#include <sistemas/ComonObjectsFactory.h>
#include <architecture/GeneralData.h>
#include <architecture/GameConstants.h>
#include <architecture/Game.h>

ecs::DeathScene::DeathScene() {

}

ecs::DeathScene::~DeathScene() {

}

void ecs::DeathScene::init() {
	factory_->setLayer(ecs::layer::BACKGROUND);
	factory_->createImage(Vector2D(0,0), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH),
		&sdlutils().images().at("blackScreen"));

	factory_->setLayer(ecs::layer::DEFAULT);
	factory_->setFont("simpleHandmade");
	factory_->createLabel(Vector2D(LOGICAL_RENDER_WIDTH / 2 - 190, 300), "HAS MUERTO", 80, 
		SDL_Color{ 255, 255, 255, 255});

	factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH / 2 - 210, 600), "CARGAR ULTIMO GUARDADO",40, []{
			gm().requestChangeScene(ecs::sc::DEATH_SCENE, ecs::sc::EXPLORE_SCENE);
			gD().loadSaveFile();
		}, "click", SDL_Color{255, 255, 255, 255});
}

void ecs::DeathScene::close() {

}