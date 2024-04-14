#include "PauseScene.h"
#include "../architecture/Entity.h"
#include <iostream>
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"
#include "../components/Render.h"
#include "../components/Clickeable.h"
#include "../architecture/Game.h"
#include "../architecture/GeneralData.h"
#include <sistemas/ComonObjectsFactory.h>


ecs::PauseScene::PauseScene() : Scene() {

}

ecs::PauseScene::~PauseScene()
{
}

void ecs::PauseScene::init()
{
	std::cout << "Hola Pausa" << std::endl;
	sdlutils().clearRenderer();

	//Font* fuente = new Font("recursos/fonts/ARIAL.ttf", 50);

	Entity* BotonPress = addEntity();
	Texture* texturaBoton = new Texture(sdlutils().renderer(), "Pulsa para volver al juego", sdlutils().fonts().at("arial50"), build_sdlcolor(0x000000ff));

	Transform* transformBoton = BotonPress->addComponent<Transform>(400, 600, texturaBoton->width(), texturaBoton->height());
	RenderImage* renderBoton = BotonPress->addComponent<RenderImage>(texturaBoton);

	Clickeable* clickerPress = BotonPress->addComponent<Clickeable>();

	
	CallbackClickeable funcPress = [this, BotonPress]() {
		try {
			//sdlutils().musics().at("pauseMenu").haltMusic();
			std::cout << "salimos de la pausa" << std::endl;
			BotonPress->setAlive(false);
			std::cout << "eliminamos el boton" << std::endl;
			gm().requestChangeScene(ecs::sc::PAUSE_SCENE, ecs::sc::NULL_SCENE);
			//leaveMenu();
		}
		catch (const std::exception& e) {
			std::cerr << "Error in funcPress callback: " << e.what() << std::endl;
		}
	};
	if (clickerPress != nullptr) {
		clickerPress->addEvent(funcPress);
	}
	else {
		std::cerr << "clickerPress is nullptr!" << std::endl;
	}
}

void ecs::PauseScene::leaveMenu() {
	gm().killScene(ecs::sc::PAUSE_SCENE);
}
