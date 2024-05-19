#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "MainMenu.h"
#include <architecture/Entity.h>
#include <iostream>
#include <sdlutils/SDLUtils.h>
#include <components/Transform.h>
#include <components/Render.h>
#include <components/Clickeable.h>
#include <architecture/Game.h>
#include <architecture/GeneralData.h>
#include <sistemas/ComonObjectsFactory.h>
#include <architecture/GameConstants.h>
#include <sistemas/SoundEmiter.h>

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

	//gD().loadSaveFile();

#ifdef _DEBUG
	std::cout << "Hola Menu" << std::endl;
#endif // _DEBUG

	//gD().updateFelicidadPersonajes();
	sdlutils().clearRenderer();

	Entity* fondo = addEntity(ecs::layer::BACKGROUND);
	Texture* texturaFondo = &sdlutils().images().at("fondoMainMenu");
	Transform* transformFondo = fondo->addComponent<Transform>(0.0f, 0.0f, LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH);
	RenderImage* renderFondo = fondo->addComponent<RenderImage>(texturaFondo);

	Entity* titulo = addEntity(ecs::layer::BACKGROUND);
	Texture* texturaTitulo = &sdlutils().images().at("title");
	Transform* transformTitulo = titulo->addComponent<Transform>(350.0f, 50.0f, texturaTitulo->width()*1.5f, texturaTitulo->height()* 1.5f);
	RenderImage* renderTitulo = titulo->addComponent<RenderImage>(texturaTitulo);

	auto textColor = build_sdlcolor(0xffffffff);
	factory_->setLayer(layer::UI);

	auto start = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 400), "Nueva partida", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gD().newGame();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::INTRO_SCENE);
		},"click", textColor);

	factory_->addHilghtOnHover(start);
	factory_->addHoverColorMod(start);

	auto loadSave = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 500), "Cargar partida guardada", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::EXPLORE_SCENE);
		try {
			gD().loadSaveFile();
		}
		catch (save_Missing e) {
			std::string god = e.what();
			god += "se cargara una nueva partida desde el dia 1";
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "ERROR",god.c_str(), sdlutils().window());
			gD().newGame();
		}
		}, "click", textColor);
	factory_->addHilghtOnHover(loadSave);
	factory_->addHoverColorMod(loadSave);

	auto finalesScene = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 600), "Finales Completados", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::FINALS_INFO_SCENE);
	}, "click", textColor);
	factory_->addHilghtOnHover(finalesScene);
	factory_->addHoverColorMod(finalesScene);

	auto ajustes = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 700), "Configuracion", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
	gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::CONFIG_SCENE);
		}, "click", textColor);
	factory_->addHilghtOnHover(ajustes);
	factory_->addHoverColorMod(ajustes);

	auto exit = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 800), "Salir", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().endGame();
		}, "click", textColor);
	factory_->addHilghtOnHover(exit);
	factory_->addHoverColorMod(exit);

	SoundEmiter::instance()->playMusic("mainMenu");
}

void ecs::MainMenu::close()
{
	ecs::Scene::close();
	SoundEmiter::instance()->close();
}

void ecs::MainMenu::changeToMainScene() {

	//game().loadScene(ecs::sc::MAIN_SCENE);

}
