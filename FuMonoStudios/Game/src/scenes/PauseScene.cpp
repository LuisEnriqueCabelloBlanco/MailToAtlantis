#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "PauseScene.h"
#include <architecture/Game.h>
#include <sistemas/ComonObjectsFactory.h>


ecs::PauseScene::PauseScene() : ConfigScene() {

}

ecs::PauseScene::~PauseScene()
{
}

void ecs::PauseScene::init()
{
	//TODO revisar esto
#ifdef _DEBUG
	std::cout << "Hola Pausa" << std::endl;
#endif // _DEBUG

	ConfigScene::init();

	CallbackClickeable funcPress = [this]() {
			gm().unpauseGame();
			gm().requestChangeScene(ecs::sc::PAUSE_SCENE, ecs::sc::NULL_SCENE);
#ifdef _DEBUG
			std::cout << "salimos de la pausa" << std::endl;
#endif // _DEBUG
	};

	factory_->createImageButton({ 0,750 }, Vector2D(375, 125), &sdlutils().images().at("cartelContinuar"), funcPress, "click");

	mainMenuButton->getComponent<Clickeable>()->deleteEvents();
	mainMenuButton->getComponent<Clickeable>()->addEvent([this]
		{
			gm().requestChangeScene(ecs::sc::PAUSE_SCENE, ecs::sc::MENU_SCENE);
			gm().killScene(ecs::sc::MAIN_SCENE);
			gm().killScene(ecs::sc::EXPLORE_SCENE);
		});
}

void ecs::PauseScene::close()
{
	clearScene();
}
