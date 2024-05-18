#include "Game.h"
#include <list>
#include <sdlutils/InputHandler.h>
#ifdef DEV_TOOLS
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#else
#include <utils/checkML.h>
#endif // DEV_TOOLS
#include <SDL.h>
#include <algorithm>
#include <scenes/MainScene.h>
#include <scenes/ConfigScene.h>
#include <scenes/MainMenu.h>
#include <scenes/PauseScene.h>
#include <scenes/ExplorationScene.h>
#include <scenes/EndWorkScene.h>
#include <scenes/EndGameScene.h>
#include <scenes/TutorialScene.h>
#include <scenes/IntroScene.h>
#include <scenes/DeathScene.h>
#include <architecture/Time.h>
#include <architecture/GeneralData.h>
#include <iostream>
#include <QATools/DataCollector.h>
#include <architecture/GameConstants.h>
#include <sistemas/SoundEmiter.h>

Game::Game() :exit_(false) {
	SDLUtils::init("Mail To Atlantis", 1152, 648, "recursos/config/mail.resources.json");
	Config::init("recursos/config/mail.config.json");
	GeneralData::init();
	PaqueteBuilder::initdata();

	auto& sdl = *SDLUtils::instance();

	sdl.showCursor();
	window_ = sdl.window();
	renderer_ = sdl.renderer();

	SDL_RenderSetLogicalSize(renderer_,LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH);

	SDL_SetWindowFullscreen(window_,SDL_WINDOW_FULLSCREEN_DESKTOP);

	gameScenes_ = { new ecs::MainScene(),new ecs::ExplorationScene(),
		new EndWorkScene(),new ecs::MainMenu(),new ecs::PauseScene(),
		new EndGameScene(),new ecs::TutorialScene(), new ecs::ConfigScene(),
		new ecs::IntroScene, new ecs::DeathScene()};
	gamePaused_ = false;

	loadScene(ecs::sc::MENU_SCENE);
	sceneChange_ = false;
}

Game::~Game()
{
	for (auto s : gameScenes_) {
		delete s;
	}
}


void Game::run()
{
	//esto es una cochinada pero mejor esto a que tarde 2 anios en cargar la escena de exploracion
	gD().readNPCData();
#ifdef DEV_TOOLS
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.DisplaySize = ImGui::GetMainViewport()->Size;
	ImGui_ImplSDL2_InitForSDLRenderer(sdlutils().window(), sdlutils().renderer());
	ImGui_ImplSDLRenderer2_Init(sdlutils().renderer());
#endif // DEV_TOOLS
	SoundEmiter::instance()->init();

	while (!exit_)
	{
		if (sceneChange_)
		{
			changeScene(scene1_, scene2_);
			sceneChange_ = false;
		}
		refresh();
		ih().refresh();
		Uint32 startTime = sdlutils().virtualTimer().currTime();

		if (ih().isKeyDown(SDL_SCANCODE_ESCAPE) || ih().closeWindowEvent()) {
			exit_ = true;
		}
		if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_F)) {
			sdlutils().toggleFullScreen();
		}

		if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_P)) {
			if (!gamePaused_) {
				loadScene(ecs::sc::PAUSE_SCENE);
				gamePaused_ = true;
			}
			else {
				changeScene(ecs::sc::PAUSE_SCENE, ecs::sc::NULL_SCENE);
				gamePaused_ = false;
			}
		}

#ifdef _DEBUG
		if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_S)) {
			gD().saveGame();
		}
#endif // _DEBUG

#ifdef QA_TOOLS
		if (ih().mouseButtonDownEvent()&&ih().getMouseButtonState(0)) {
			dataCollector().clicks()++;
		}
#endif // QA_TOOLS

		update();
		sdlutils().clearRenderer(build_sdlcolor(0x000000));
#ifdef DEV_TOOLS
		ImGui_ImplSDLRenderer2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
#endif // DEV_TOOLS

		/**/
		render();

		sdlutils().presentRenderer();

		Time::deltaTime_ = (sdlutils().virtualTimer().currTime() - startTime) / 1000.0;
	}
#ifdef DEV_TOOLS
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
#endif // DEV_TOOLS
}

/// <summary>
/// Carga la escena indicada por el Id
/// Se ejecutara la ultima de la cadena de proceso
/// </summary>
/// <param name="scene"></param>

void Game::loadScene(ecs::sc::sceneId scene)
{
#ifdef QA_TOOLS
	dataCollector().dataArray()[0] = (int)scene;
#endif // QA_TOOLS

	auto it = std::find(loadedScenes_.begin(), loadedScenes_.end(), gameScenes_[scene]);
	if (it == loadedScenes_.end()) {
		//llamar al init de la escena a cargar
		gameScenes_[scene]->init();
		//cargamos la escena
		loadedScenes_.push_back(gameScenes_[scene]);
#ifdef _DEBUG
		std::cout << "Scene "<<(int) scene<< " Loaded" << std::endl;
#endif // _DEBUG

	}
#ifdef QA_TOOLS
	dataCollector().record();
#endif // QA_TOOLS

}

/// <summary>
/// Descarga al escena pedida
/// </summary>
/// <param name="scene"></param>
void Game::killScene(ecs::sc::sceneId scene)
{
	auto it = std::find(loadedScenes_.begin(), loadedScenes_.end(), gameScenes_[scene]);
	if (it != loadedScenes_.end()) {
		(*it)->close();
		loadedScenes_.erase(it);
#ifdef _DEBUG
		std::cout << "Scene " << (int) scene << " Killed" << std::endl;
#endif // _DEBUG

	}
}

void Game::requestChangeScene(ecs::sc::sceneId scene1, ecs::sc::sceneId scene2)
{
	sceneChange_ = true;
	scene1_ = scene1;
	scene2_ = scene2;
}

void Game::changeScene(ecs::sc::sceneId scene1, ecs::sc::sceneId scene2) {
	//Estas comprobaciones van a ser una prueba de que se puede modificar la clase GeneralData, no estará así en la versión final
	if (scene1 == ecs::sc::MENU_SCENE) {
		gD().setFinalID(1);
	}
	else if (scene1 == ecs::sc::EXPLORE_SCENE) {
		gD().setFinalID(2);
		gD().setTubesAmount(gD().getPlacesToActive().size());
	}
	else if (scene1 == ecs::sc::MAIN_SCENE) {
		gD().setFinalID(3);
	}
	killScene(scene1);
	if (scene2 != ecs::sc::NULL_SCENE) {
		loadScene(scene2);
	}
	/*if (loadedScenes.size() < 1) {
		loadScene(scene2);
	}*/
}

/// <summary>
/// Actualiza el juego
/// </summary>
void Game::update()
{
	for (auto& scene : loadedScenes_) {
		scene->update();
	}
}
/// <summary>
/// renderiza en pantalla el juego
/// no llamar a render clear o present se llamaran antes y despues de la llamada a este metodo
/// </summary>
void Game::render()
{
	for (auto& scene : loadedScenes_) {
		scene->render();
	}
}

void Game::refresh()
{
	for (auto& scene : loadedScenes_) {
		scene->refresh();
	}
}
