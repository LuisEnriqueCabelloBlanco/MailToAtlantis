#include "Game.h"
#include <list>
#include <SDL.h>
#include "../sdlutils/InputHandler.h"
/*
TODO
A�adir fichero de configuracion el init de SDLUtils cuando haya recursos que cargar
*/

Game::Game():exit(false) {
	SDLUtils::init("Mail To Atlantis", 800, 600);

	auto& sdl = *SDLUtils::instance();

	sdl.showCursor();
	window = sdl.window();
	renderer = sdl.renderer();

}

Game::~Game()
{

}


void Game::run()
{
	while (!exit)
	{
		ih().refresh();
		Uint32 startTime = SDL_GetTicks();

		if (ih().keyDownEvent() || ih().closeWindowEvent()) {
			exit = true;
		}

		update();

		render();

		Uint32 frameTime = SDL_GetTicks() - startTime;

		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}
}

void Game::update()
{
	for (auto& scene : loadedScenes) {
		scene->update();
	}	
}

void Game::render()
{
	for (auto& scene : loadedScenes) {
		scene->render();
	}
}
