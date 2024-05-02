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
#include <components/HoverSensorComponent.h>

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
#ifdef _DEBUG
	std::cout << "Hola Menu" << std::endl;
#endif // _DEBUG

	//generalData().updateFelicidadPersonajes();
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
	auto tuto = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH- 700, 400), "Tutorial", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::TUTORIAL_SCENE);
		},textColor);
	factory_->addHilghtOnHover(tuto);
	factory_->addHoverColorMod(tuto);


	auto start = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 500), "Nueva partida", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::EXPLORE_SCENE);
		},textColor);
	factory_->addHilghtOnHover(start);
	factory_->addHoverColorMod(start);

	auto loadSave = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 600), "Cargar partida guardada", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::EXPLORE_SCENE);
		generalData().loadSaveFile();
		}, textColor);
	factory_->addHilghtOnHover(loadSave);
	factory_->addHoverColorMod(loadSave);

	auto ajustes = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 700), "Configuracion", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
	gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::CONFIG_SCENE);
		}, textColor);
	factory_->addHilghtOnHover(ajustes);
	factory_->addHoverColorMod(ajustes);

	auto exit = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 800), "Salir", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().endGame();
		},textColor);
	factory_->addHilghtOnHover(exit);
	factory_->addHoverColorMod(exit);
}

void ecs::MainMenu::changeToMainScene() {

	//game().loadScene(ecs::sc::MAIN_SCENE);

}
