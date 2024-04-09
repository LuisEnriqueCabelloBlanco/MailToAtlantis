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


	factory_->createTextuButton(Vector2D(600, 600), "Pulsa para empezar", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::EXPLORE_SCENE);
		});

	factory_->createTextuButton(Vector2D(600, 500), "Tutorial", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::TUTORIAL_SCENE);
		});
}

void ecs::MainMenu::changeToMainScene() {

	//game().loadScene(ecs::sc::MAIN_SCENE);

}
