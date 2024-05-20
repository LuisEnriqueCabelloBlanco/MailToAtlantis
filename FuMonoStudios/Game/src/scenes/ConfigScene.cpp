#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "ConfigScene.h"
#include <iostream>
#include <sdlutils/SDLUtils.h>
#include <components/Transform.h>
#include <components/Render.h>
#include <architecture/Game.h>
#include <architecture/GeneralData.h>
#include <sistemas/ComonObjectsFactory.h>
#include <architecture/GameConstants.h>


ecs::ConfigScene::ConfigScene() : Scene() {

}

ecs::ConfigScene::~ConfigScene()
{
}

void ecs::ConfigScene::init()
{
#ifdef _DEBUG
	std::cout << "Hola Config" << std::endl;
#endif // _DEBUG

	sdlutils().clearRenderer();
	factory_->setLayer(ecs::layer::BACKGROUND);
	factory_->createImage(Vector2D(0,0), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH), &sdlutils().images().at("fondoAjustes"));
	factory_->setLayer(ecs::layer::DEFAULT);

	//boton para volver al menu
	CallbackClickeable funcPress = [this]() {
		gm().requestChangeScene(ecs::sc::CONFIG_SCENE, ecs::sc::MENU_SCENE);

	};
	factory_->createTextuButton({ 10,930 }, "                          ", 50, funcPress, "click");

	factory_->setLayer(ecs::layer::UI);
	factory_->setFont("hvdComicSerif");
	createMusicOptions();
	createSFXOptions();
	createFullscreenOptions();
	createSkipTutorialOptions();
	factory_->setLayer(ecs::layer::DEFAULT);
}

void ecs::ConfigScene::createMusicOptions()
{
	//iconito del sello
	musicIconTexture_ = &sdlutils().images().at("iconoRojoAjustes");
	musicIconEnt_ = factory_->createImage(Vector2D(560 + (gD().getParamMusic() * 7.4f), 330), Vector2D(musicIconTexture_->width(), musicIconTexture_->height()), musicIconTexture_);

	// Boton (-) para el parametro de audio musica
	CallbackClickeable funcPress2 = [this]() {
		gD().changeParamID(0, false);
		musicIconEnt_->getComponent<Transform>()->setPos(550 + (gD().getParamMusic() * 7.4f), 330);
		};
	Entity* aux = factory_->createTextuButton({ 570,380 }, "-", 50, funcPress2, "click");
	// Boton (+) para el parametro de audio musica
	CallbackClickeable funcPress3 = [this]() {
		gD().changeParamID(0, true);
		musicIconEnt_->getComponent<Transform>()->setPos(550 + (gD().getParamMusic() * 7.4f), 330);
		};
	factory_->createTextuButton({ 1300,380 }, "+", 50, funcPress3, "click");
	factory_->createLabel(Vector2D(670, 280), "Volumen de la musica", 50);
}

void ecs::ConfigScene::createSFXOptions()
{
	sfxIconTexture_ = &sdlutils().images().at("iconoAzulAjustes");
	sfxIconEnt_ = factory_->createImage(Vector2D(560 + (gD().getParamSfx() * 7.4f), 500), Vector2D(sfxIconTexture_->width(), sfxIconTexture_->height()), sfxIconTexture_);

	// Boton (-) para el parametro de audio sfx
	CallbackClickeable funcPress4 = [this]() {
		gD().changeParamID(1, false);
		sfxIconEnt_->getComponent<Transform>()->setPos(550 + (gD().getParamSfx() * 7.4f), 500);
		};
	auto minus = factory_->createTextuButton({ 570,560 }, "-", 50, funcPress4, "click",build_sdlcolor("0x000000ff"));
	factory_->addHoverColorMod(minus,build_sdlcolor("0xffffffff"));
	// Boton (+) para el parametro de audio sfx
	CallbackClickeable funcPress5 = [this]() {
		gD().changeParamID(1, true);
		sfxIconEnt_->getComponent<Transform>()->setPos(550 + (gD().getParamSfx() * 7.4f), 500);
		};
	auto plus = factory_->createTextuButton({ 1300,560 }, "+", 50, funcPress5, "click", build_sdlcolor("0x000000ff"));
	factory_->addHoverColorMod(plus, build_sdlcolor("0xffffffff"));
	factory_->createLabel(Vector2D(670,460), "Volumen de los efectos", 50);
}

void ecs::ConfigScene::createFullscreenOptions()
{
	//screenModeIconTexture_ = &sdlutils().images().at("iconoVerdeAjustes");
	std::vector<Texture*> textures = { &sdlutils().images().at("iconoVerdeAjustes"),nullptr };
	Transform* trBase = factory_->createImage(Vector2D(-10,60),&sdlutils().images().at("tinta2"))->getComponent<Transform>();
	screenModeIconEnt_ = factory_->createMultiTextureImage(Vector2D(585, 665), Vector2D(95, 122), textures);
	trBase->setParent(screenModeIconEnt_->getComponent<Transform>());

	screenModeIconEnt_->getComponent<RenderImage>()->setNumberTexture(!gD().GetValueFullScreen()?0:1);

	CallbackClickeable funcScreenModeBoton = [this]() {
		sdlutils().toggleFullScreen();
		gD().ToggleFullScreen();
		screenModeIconEnt_->getComponent<RenderImage>()->setNumberTexture(!gD().GetValueFullScreen()?0:1);
	};
	factory_->createTextuButton({ 740,730 }, "Pantalla Completa", 50, funcScreenModeBoton, "click");
}

void ecs::ConfigScene::createSkipTutorialOptions()
{
	//skipTutoIconTexture_ = &sdlutils().images().at("iconoAmarilloAjustes");
	std::vector<Texture*> textures = { &sdlutils().images().at("iconoAmarilloAjustes"),nullptr };
	Transform* trBase = factory_->createImage(Vector2D(-10, 60), &sdlutils().images().at("tinta2"))->getComponent<Transform>();
	skipTutoIconEnt_ = factory_->createMultiTextureImage(Vector2D(585, 850), Vector2D(95, 122), textures);
	trBase->setParent(skipTutoIconEnt_->getComponent<Transform>());
	skipTutoIconEnt_->getComponent<RenderImage>()->setNumberTexture(!gD().GetValueSkipTutorial() ? 0 : 1);

	CallbackClickeable funcPressSkipTutorial = [this]() {
		gD().ToggleSkipTutorial();
		skipTutoIconEnt_->getComponent<RenderImage>()->setNumberTexture(!gD().GetValueSkipTutorial()?0:1);
	};
	factory_->createTextuButton({ 740,920 }, "Skip Tutorial", 50, funcPressSkipTutorial, "click");
}

