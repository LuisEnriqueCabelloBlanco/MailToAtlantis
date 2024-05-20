#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "MainMenu.h"
#include <components/Render.h>
#include <architecture/Game.h>
#include <architecture/GeneralData.h>
#include <sistemas/ComonObjectsFactory.h>
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
#ifdef _DEBUG
	std::cout << "Hola Menu" << std::endl;
#endif // _DEBUG

	sdlutils().clearRenderer();

	Entity* fondo = addEntity(ecs::layer::BACKGROUND);
	Texture* texturaFondo = &sdlutils().images().at("menuPrincipal");
	fondo->addComponent<Transform>(0.0f, 0.0f, LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH);
	fondo->addComponent<RenderImage>(texturaFondo);

	auto textColor = build_sdlcolor(0x666666ff);
	factory_->setFont("simpleHandmade");
	factory_->setLayer(layer::UI);

	auto start = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 500), "Nueva partida", 60, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gD().newGame();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::INTRO_SCENE);
		},"click", textColor);

    factory_->addHoverColorMod(start);

	auto loadSave = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 600), "Cargar partida guardada", 60, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::EXPLORE_SCENE);
		try {
			gD().loadSaveFile();
		}
		catch (save_Missing& e) {
			std::string god = e.what();
			god += "se cargara una nueva partida desde el dia 1";
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "ERROR",god.c_str(), sdlutils().window());
			gD().newGame();
		}
		}, "click", textColor);

	factory_->addHoverColorMod(loadSave);

	auto endScene = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 700), "Mostramos Final Juego", 60, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::END_SCENE);
	}, "click", textColor);

    factory_->addHoverColorMod(endScene);

	auto ajustes = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 800), "Configuracion", 60, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
	gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::CONFIG_SCENE);
		}, "click", textColor);

    factory_->addHoverColorMod(ajustes);

	auto exit = factory_->createTextuButton(Vector2D(LOGICAL_RENDER_WIDTH - 700, 900), "Salir", 60, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gm().endGame();
		}, "click", textColor);

    factory_->addHoverColorMod(exit);

	SoundEmiter::instance()->playMusic("mainMenu");
	factory_->setFont("arial");
}

void ecs::MainMenu::close()
{
	ecs::Scene::close();
	SoundEmiter::instance()->close();
}

