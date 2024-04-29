#include <utils/checkML.h>
#include "ConfigScene.h"
#include "../architecture/Entity.h"
#include <iostream>
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"
#include "../components/Render.h"
#include "../components/Clickeable.h"
#include "../architecture/Game.h"
#include <string>
#include "../architecture/GeneralData.h"
#include <sistemas/ComonObjectsFactory.h>

//ecs::MainMenu::MainMenu()
//{
//	init();
//}

ecs::ConfigScene::ConfigScene() : Scene() {

}

ecs::ConfigScene::~ConfigScene()
{
}

void ecs::ConfigScene::init()
{
	std::cout << "Hola Config" << std::endl;
	sdlutils().clearRenderer();

	//boton para volver al menu
	CallbackClickeable funcPress = [this]() {
		gm().requestChangeScene(ecs::sc::CONFIG_SCENE, ecs::sc::MENU_SCENE);

	};
	factory_->createTextuButton({ 400,400 }, "Pulsa para volver al menu", 50,funcPress);


	//Luis: esto que sea place holder hay que poner imagen

	// Boton cambio de pantalla
	CallbackClickeable funcScreenModeBoton = [this]() {
		sdlutils().toggleFullScreen();
	};
	factory_->createTextuButton({ 500,700 }, "Cambiar modo de ventana", 50, funcScreenModeBoton);

	// Parametro de audio
	audioValueTexture_ = new Texture(sdlutils().renderer(), std::to_string(generalData().getParam(1)), sdlutils().fonts().at("arial50"), build_sdlcolor(0x000000ff));
	audioValueEnt_ = addEntity();
	Transform* transformAudioValue = audioValueEnt_->addComponent<Transform>(500, 600, audioValueTexture_->width(), audioValueTexture_->height());
	RenderImage* renderAudioValue = audioValueEnt_->addComponent<RenderImage>(audioValueTexture_);
	
	// Boton (-) para el parametro de audio
	CallbackClickeable funcPress2 = [this]() {
		generalData().changeParamID(1, false);
		//updateValue(audioValueTexture_, audioValueEnt_, 1);
		if (audioValueTexture_ != nullptr)
		{
			delete audioValueTexture_;
			audioValueTexture_ = nullptr;
		}

		audioValueTexture_ = new Texture(sdlutils().renderer(), std::to_string(generalData().getParam(1)), sdlutils().fonts().at("arial50"), build_sdlcolor(0x000000ff));
		audioValueEnt_->getComponent<RenderImage>()->setTexture(audioValueTexture_);

	};
	factory_->createTextuButton({ 400,600 }, "(-)", 50, funcPress2);

	// Boton (+) para el parametro de audio
	factory_->createTextuButton({ 600,600 }, "(+)", 50, funcPress2);
}

//No usar de momento este m�todo porque peta el programa aunque tenga la misma funci�n que lo que pasa en los callbacks
void ecs::ConfigScene::updateValue(Texture* texture, Entity* entity, int index) {

	if (texture != nullptr)
	{
		delete texture;
		texture = nullptr;
	}

	texture = new Texture(sdlutils().renderer(), std::to_string(generalData().getParam(index)), sdlutils().fonts().at("arial50"), build_sdlcolor(0x000000ff));
	entity->getComponent<RenderImage>()->setTexture(texture);
}
//LUIS: Y si esto fuera un return to scene ????
void ecs::ConfigScene::changeToMenuScene() {

	//game().loadScene(ecs::sc::MAIN_SCENE);

}
