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
	factory_->setFont("hvdComicSerif");
	factory_->setLayer(layer::UI);

	Vector2D pos(LOGICAL_RENDER_WIDTH - 725, 500);

	auto start = factory_->createTextuButton(pos, "Nueva partida", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
		gD().newGame();
		gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::INTRO_SCENE);
		},"click", textColor);
	pos.setY(pos.getY()+100);
    factory_->addHoverColorMod(start);

	auto loadSave = factory_->createTextuButton(pos, "Cargar partida guardada", 50, [this]() {
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
	pos.setY(pos.getY() + 100);
	factory_->addHoverColorMod(loadSave);


	if (gD().hasGameEndedOnce()) {
		auto endScene = factory_->createTextuButton(pos, "Mostramos Final Juego", 50, [this]() {
			sdlutils().musics().at("mainMenu").haltMusic();
			gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::END_SCENE);
		}, "click", textColor);
		pos.setY(pos.getY() + 100);
		factory_->addHoverColorMod(endScene);
	}


	auto ajustes = factory_->createTextuButton(pos, "Configuracion", 50, [this]() {
		sdlutils().musics().at("mainMenu").haltMusic();
	gm().requestChangeScene(ecs::sc::MENU_SCENE, ecs::sc::CONFIG_SCENE);
		}, "click", textColor);
	pos.setY(pos.getY() + 100);
    factory_->addHoverColorMod(ajustes);

	auto exit = factory_->createTextuButton(pos, "Salir", 50, [this]() {
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

