#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "PauseScene.h"
#include <architecture/Game.h>
#include <sistemas/ComonObjectsFactory.h>
#include <sistemas/SoundEmiter.h> 


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

	factory_->setLayer(ecs::layer::BACKGROUND);
	factory_->createImage(Vector2D(0, 0), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH), &sdlutils().images().at("fondoPausa"));
	factory_->setLayer(ecs::layer::DEFAULT);

	CallbackClickeable funcPress = [this/*, BotonPress*/]() {
#ifdef _DEBUG
			std::cout << "eliminamos el boton" << std::endl;
#endif // _DEBUG
			gm().unpauseGame();
			gm().requestChangeScene(ecs::sc::PAUSE_SCENE, ecs::sc::NULL_SCENE);
#ifdef _DEBUG
			std::cout << "salimos de la pausa" << std::endl;
#endif // _DEBUG
	};

	CallbackClickeable exitToMenu = [this/*, BotonPress*/]() {
#ifdef _DEBUG
			std::cout << "eliminamos el boton" << std::endl;
#endif // _DEBUG
			gm().unpauseGame();
			gm().requestChangeScene(ecs::sc::PAUSE_SCENE, ecs::sc::MENU_SCENE);
			gm().killScene(ecs::sc::EXPLORE_SCENE);
			gm().killScene(ecs::sc::MAIN_SCENE);

#ifdef _DEBUG
			std::cout << "salimos de la pausa" << std::endl;
#endif // _DEBUG
	};

	factory_->createTextuButton({ 10,730 }, "                          ", 50, exitToMenu, "click");
	factory_->createTextuButton({ 10,930 }, "                          ", 50, funcPress, "click");

	SoundEmiter::instance()->playMusic("printer");
}

