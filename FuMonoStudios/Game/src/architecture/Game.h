#pragma once
#include<list>
#include <SDL.h>
#include <array>
#include <utils/Singleton.h>
#include <architecture/ecs.h>
#include <sdlutils/SDLUtils.h>
#include "Config.h"
#include <architecture/Scene.h>
#include <functional>



#ifdef _DEBUG
#define DEV_TOOLS
#endif // _DEBUG
#define QA_TOOLS






class Game : public Singleton<Game> {

	friend Singleton<Game>;

public:
	Game();
	~Game();
	void run();
	/// <summary>
	/// Metodo para cargar una escena
	/// </summary>
	/// <param name="scene">Id de la escena a cargar</param>
	void loadScene(ecs::sc::sceneId scene);
	/// <summary>
	/// Metodo para descargar una escena
	/// </summary>
	/// <param name="scene">Id de la escena a matar</param>
	void killScene(ecs::sc::sceneId scene);

	/// <summary>
	/// Metodo para solicitar cambio de una escena a otra
	/// </summary>
	/// <param name="scene1"></param>
	/// <param name="scene2"></param>
	void requestChangeScene(ecs::sc::sceneId scene1, ecs::sc::sceneId scene2);
	void unpauseGame() {
		gamePaused_ = false;
	}
	bool gamePaused() {
		return gamePaused_;
	}
	inline void writeMessage() {
		std::cout << "Funcionaaaaaaaaa" << std::endl;
	};
	void endGame() { exit_ = true; }
private:
	/// <summary>
	/// Metodo para cambiar de una escena a otra
	/// </summary>
	/// <param name="scene1"></param>
	/// <param name="scene2"></param>
	void changeScene(ecs::sc::sceneId scene1, ecs::sc::sceneId scene2);

	void update();
	void render();
	void refresh();

	/// <summary>
	/// Lista de escenas a procesar
	/// </summary>
	std::vector<ecs::Scene*> loadedScenes_;
	/// <summary>
	/// Lista de escenas del juego
	/// </summary>
	std::array<ecs::Scene*, ecs::sc::maxSceneId> gameScenes_;
	bool exit_;
	SDL_Window* window_;
	SDL_Renderer* renderer_;

	bool sceneChange_;
	bool gamePaused_;
	ecs::sc::sceneId scene1_, scene2_; //una guarrada lo se, pero ahora mismo quiero que el juego no explote, ya se hara bonito


	std::list<std::function<void(void)>> requets;

	float autoRecodTime = 0;
};

inline Game& gm() {
	return *Game::instance();
}
