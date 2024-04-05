#include "MainMenu.h"
#include "../architecture/Entity.h"
#include <iostream>
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"
#include "../components/Render.h"
#include "../components/Clickeable.h"
#include "../architecture/Game.h"
#include "../architecture/GeneralData.h"
#include "../components/RenderWithLight.h"
#include <sistemas/ComonObjectsFactory.h>

//ecs::MainMenu::MainMenu()
//{
//	init();
//}

ecs::MainMenu::MainMenu() : Scene() {

}

ecs::MainMenu::~MainMenu()
{
}

void ecs::MainMenu::init()
{
	std::cout << "Hola Menu" << std::endl;
	//generalData().updateFelicidadPersonajes();
	sdlutils().clearRenderer();

	//Font* fuente = new Font("recursos/fonts/ARIAL.ttf", 50);
	Entity* fondo = addEntity();
	Texture* texturaFondo = &sdlutils().images().at("fondoMainMenu");
	Transform* transformFondo = fondo->addComponent<Transform>(0.0f, 0.0f, texturaFondo->width()* 1.6f, texturaFondo->height()* 1.6f);
	RenderImage* renderFondo = fondo->addComponent<RenderImage>(texturaFondo);

	Entity* titulo = addEntity();
	Texture* texturaTitulo = &sdlutils().images().at("title");
	Transform* transformTitulo = titulo->addComponent<Transform>(350.0f, 50.0f, texturaTitulo->width()*1.5f, texturaTitulo->height()* 1.5f);
	RenderImage* renderTitulo = titulo->addComponent<RenderImage>(texturaTitulo);

	Texture* texturaBoton = new Texture(sdlutils().renderer(), "Pulsa para empezar", sdlutils().fonts().at("arial50"), build_sdlcolor(0x000000ff));
	Entity* BotonPress = addEntity();
	
	Transform* transformBoton = BotonPress->addComponent<Transform>(600, 600, texturaBoton->width(), texturaBoton->height());
	RenderWithLight* renderBoton = BotonPress->addComponent<RenderWithLight>(texturaBoton);

	auto clickerPress = BotonPress->addComponent<Clickeable>();

	/*Boton->addComponent<Trigger>();
	Boton->getComponent<Trigger>()->addCallback([]() {

		std::cout << "Tocando" << std::endl;

		});*/

	/*void Aux(Game * game) {

		

		(*game).loadScene(ecs::sc::MAIN_SCENE);

	}*/
	CallbackClickeable funcPress = [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::EXPLORE_SCENE);

	};
	clickerPress->addEvent(funcPress);
	sdlutils().musics().at("mainMenu").play();
}

void ecs::MainMenu::changeToMainScene() {

	//game().loadScene(ecs::sc::MAIN_SCENE);

}
