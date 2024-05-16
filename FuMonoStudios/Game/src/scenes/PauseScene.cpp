#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "PauseScene.h"
#include <architecture/Entity.h>
#include <iostream>
#include <sdlutils/SDLUtils.h>
#include <components/Transform.h>
#include <components/Render.h>
#include <components/Clickeable.h>
#include <architecture/Game.h>
#include <architecture/GeneralData.h>
#include <sistemas/ComonObjectsFactory.h>


ecs::PauseScene::PauseScene() : Scene() {

}

ecs::PauseScene::~PauseScene()
{
}

void ecs::PauseScene::init()
{
	//TODO revisar esto
	std::cout << "Hola Pausa" << std::endl;
	sdlutils().clearRenderer();

	
	CallbackClickeable funcPress = [this/*, BotonPress*/]() {
		try {
			//BotonPress->setAlive(false);
#ifdef _DEBUG
			std::cout << "eliminamos el boton" << std::endl;
#endif // _DEBUG
			gm().unpauseGame();
			gm().requestChangeScene(ecs::sc::PAUSE_SCENE, ecs::sc::NULL_SCENE);
#ifdef _DEBUG
			std::cout << "salimos de la pausa" << std::endl;
#endif // _DEBUG

		}
		catch (const std::exception& e) {
			std::cerr << "Error in funcPress callback: " << e.what() << std::endl;
		}
	};
	Entity* BotonPress = factory_->createTextuButton(Vector2D(400,600), "Pulsa para volver al juego",50,funcPress);
}
