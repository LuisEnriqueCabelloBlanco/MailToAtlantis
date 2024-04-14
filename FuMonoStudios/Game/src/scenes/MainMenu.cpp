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
	Transform* transformFondo = fondo->addComponent<Transform>(0.0f, 0.0f, LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH);
	RenderImage* renderFondo = fondo->addComponent<RenderImage>(texturaFondo);

	Entity* titulo = addEntity();
	Texture* texturaTitulo = &sdlutils().images().at("title");
	Transform* transformTitulo = titulo->addComponent<Transform>(350.0f, 50.0f, texturaTitulo->width()*1.5f, texturaTitulo->height()* 1.5f);
	RenderImage* renderTitulo = titulo->addComponent<RenderImage>(texturaTitulo);

	auto textColor = build_sdlcolor(0xffffffff);

	factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH- 700, 400), "Tutorial", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::TUTORIAL_SCENE);
		},textColor);

	factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 500), "Pulsa para empezar", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::EXPLORE_SCENE);
		},textColor);

	};
	clickerPress->addEvent(funcPress);

	Entity* BotonPress2 = addEntity();

	Transform* transformBoton2 = BotonPress2->addComponent<Transform>(400, 650, texturaBoton->width(), texturaBoton->height());
	RenderImage* renderBoton2 = BotonPress2->addComponent<RenderImage>(texturaBoton);

	auto clickerPress2 = BotonPress2->addComponent<Clickeable>();

	CallbackClickeable funcPress2 = [this]() {
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::CONFIG_SCENE);

	};
	clickerPress2->addEvent(funcPress2);
	factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 600), "Salir", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().endGame();
		},textColor);
}

void ecs::MainMenu::changeToMainScene() {

	//game().loadScene(ecs::sc::MAIN_SCENE);

}
