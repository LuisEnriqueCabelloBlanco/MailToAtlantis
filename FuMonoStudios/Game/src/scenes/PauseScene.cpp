#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "PauseScene.h"
#include <architecture/Entity.h>
#include <iostream>
#include <sdlutils/SDLUtils.h>
#include <components/Transform.h>
#include <components/Render.h>
#include <components/Clickeable.h>
#include <architecture/Game.h>
#include <architecture/GeneralData.h>
#include <sistemas/ComonObjectsFactory.h>


ecs::PauseScene::PauseScene() : Scene() {

}

ecs::PauseScene::~PauseScene()
{
}

void ecs::PauseScene::init()
{
	//TODO revisar esto
	std::cout << "Hola Pausa" << std::endl;
	sdlutils().clearRenderer();


	Entity* fondo = addEntity(ecs::layer::BACKGROUND);
	Texture* texturaFondo = &sdlutils().images().at("fondoAjustes");
	Transform* transformFondo = fondo->addComponent<Transform>(0.0f, 0.0f, LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH);
	RenderImage* renderFondo = fondo->addComponent<RenderImage>(texturaFondo);


	CallbackClickeable funcPress = [this/*, BotonPress*/]() {
		try {
#ifdef _DEBUG
			std::cout << "eliminamos el boton" << std::endl;
#endif // _DEBUG
			gm().unpauseGame();
			gm().requestChangeScene(ecs::sc::PAUSE_SCENE, ecs::sc::NULL_SCENE);
#ifdef _DEBUG
			std::cout << "salimos de la pausa" << std::endl;
#endif // _DEBUG

		}
		catch (const std::exception& e) {
			std::cerr << "Error in funcPress callback: " << e.what() << std::endl;
		}
	};
	factory_->createTextuButton({ 600,300 }, "Pulsa para volver al juego", 50, funcPress, "click");

	musicIconTexture_ = &sdlutils().images().at("iconoRojoAjustes");
	musicIconEnt_ = addEntity();
	sfxIconTexture_ = &sdlutils().images().at("iconoAzulAjustes");
	sfxIconEnt_ = addEntity();

	screenModeIconTexture_ = &sdlutils().images().at("iconoVerdeAjustes");
	screenModeIconEnt_ = addEntity();
	skipTutoIconTexture_ = &sdlutils().images().at("iconoAmarilloAjustes");
	skipTutoIconEnt_ = addEntity();



	// Parametro de audio musica
	Transform* trMusicIcon = musicIconEnt_->addComponent<Transform>(540, 370, musicIconTexture_->width(), musicIconTexture_->height());
	musicIconEnt_->getComponent<Transform>()->setPos(540 + (gD().getParamMusic() * 7.4f), 370);
	RenderImage* rdrMusicIcon = musicIconEnt_->addComponent<RenderImage>(musicIconTexture_);

	// Boton (-) para el parametro de audio musica
	CallbackClickeable funcPress2 = [this]() {
		gD().changeParamID(0, false);
		musicIconEnt_->getComponent<Transform>()->setPos(540 + (gD().getParamMusic() * 7.4f), 370);
	};
	factory_->createTextuButton({ 540,420 }, "    ", 50, funcPress2, "click");

	// Boton (+) para el parametro de audio musica
	CallbackClickeable funcPress3 = [this]() {
		gD().changeParamID(0, true);
		musicIconEnt_->getComponent<Transform>()->setPos(540 + (gD().getParamMusic() * 7.4f), 370);
	};
	factory_->createTextuButton({ 1280,420 }, "    ", 50, funcPress3, "click");



	// Parametro de audio sfx
	Transform* trSfxIcon = sfxIconEnt_->addComponent<Transform>(540, 540, sfxIconTexture_->width(), sfxIconTexture_->height());
	sfxIconEnt_->getComponent<Transform>()->setPos(540 + (gD().getParamSfx() * 7.4f), 540);
	RenderImage* rdrSfxIcon = sfxIconEnt_->addComponent<RenderImage>(sfxIconTexture_);

	// Boton (-) para el parametro de audio sfx
	CallbackClickeable funcPress4 = [this]() {
		gD().changeParamID(1, false);
		sfxIconEnt_->getComponent<Transform>()->setPos(540 + (gD().getParamSfx() * 7.4f), 540);
	};
	factory_->createTextuButton({ 540,590 }, "    ", 50, funcPress4, "click");

	// Boton (+) para el parametro de audio sfx
	CallbackClickeable funcPress5 = [this]() {
		gD().changeParamID(1, true);
		sfxIconEnt_->getComponent<Transform>()->setPos(540 + (gD().getParamSfx() * 7.4f), 540);
	};
	factory_->createTextuButton({ 1280,590 }, "    ", 50, funcPress5, "click");



	// Boton cambio de activar o no salto del tutorial
	Transform* trSkipIcon = skipTutoIconEnt_->addComponent<Transform>(515, 860, 95, 122);
	RenderImage* rdrSkipIcon = skipTutoIconEnt_->addComponent<RenderImage>(skipTutoIconTexture_);

	if (!gD().GetValueSkipTutorial()) {
		skipTutoIconEnt_->getComponent<RenderImage>()->setTexture(
			new Texture(sdlutils().renderer(), "           ", sdlutils().fonts().at("arial50"), build_sdlcolor(0x000000ff)));
	}

	// Boton para configurar el salto del tutorial o no
	CallbackClickeable funcPressSkipTutorial = [this]() {
		gD().ToggleSkipTutorial();

		if (!gD().GetValueSkipTutorial()) {
			skipTutoIconEnt_->getComponent<RenderImage>()->setTexture(
				new Texture(sdlutils().renderer(), "           ", sdlutils().fonts().at("arial50"), build_sdlcolor(0x000000ff)));
		}
		else skipTutoIconEnt_->getComponent<RenderImage>()->setTexture(skipTutoIconTexture_);
	};

	factory_->createTextuButton({ 480,940 }, "           ", 50, funcPressSkipTutorial, "click");



	// Boton cambio de pantalla
	Transform* trScreenIcon = screenModeIconEnt_->addComponent<Transform>(515, 685, 95, 122);
	RenderImage* rdrScreenIcon = screenModeIconEnt_->addComponent<RenderImage>(screenModeIconTexture_);

	if (!gD().GetValueFullScreen()) {
		screenModeIconEnt_->getComponent<RenderImage>()->setTexture(
			new Texture(sdlutils().renderer(), "           ", sdlutils().fonts().at("arial50"), build_sdlcolor(0x000000ff)));
	}

	CallbackClickeable funcScreenModeBoton = [this]() {
		sdlutils().toggleFullScreen();
		gD().ToggleFullScreen();
		if (!gD().GetValueFullScreen()) {
			screenModeIconEnt_->getComponent<RenderImage>()->setTexture(
				new Texture(sdlutils().renderer(), "           ", sdlutils().fonts().at("arial50"), build_sdlcolor(0x000000ff)));
		}
		else screenModeIconEnt_->getComponent<RenderImage>()->setTexture(screenModeIconTexture_);
	};
	factory_->createTextuButton({ 480,760 }, "           ", 50, funcScreenModeBoton, "click");
}
