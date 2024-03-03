#include "ExplorationScene.h"
#include "../architecture/Entity.h"
#include <iostream>
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"
#include "../components/Render.h"
#include "../components/Clickeable.h"
#include "../components/DragAndDrop.h"
#include "../components/Trigger.h"
#include "../architecture/Game.h"
#include <string>
#include "../sdlutils/Texture.h"
#include "../components/DialogComponent.h"

ecs::ExplorationScene::ExplorationScene():Scene(), dialogMngr_(), map()
{
	dialogMngr_.setDialogues("resources/dialogos/dialogo.txt");
}

ecs::ExplorationScene::~ExplorationScene()
{
}

void ecs::ExplorationScene::init()
{
	std::cout << "Hola Exploracion"<<std::endl;
	sdlutils().clearRenderer();
	// Caja CLicker
	Entity* Prueba2 = addEntity();
	Texture* sujetaplazas = &sdlutils().images().at("boxTest");
	float scale = 0.2;
	Transform* e = Prueba2->addComponent<Transform>(700.0f, 100.0f, sujetaplazas->width() * scale, sujetaplazas->height() * scale);
	RenderImage* nachos = Prueba2->addComponent<RenderImage>(sujetaplazas);
	auto clicker = Prueba2->addComponent<Clickeable>();
	Prueba2->addComponent<Trigger>();


	/*Prueba2->getComponent<Trigger>()->addCallback([this]() {
		std::cout << "a" << std::endl;
		map.navigate("Hermes");
		sdlutils().clearRenderer();
		map.render();
		});*/

		//TODO: probar que con un boton se puedan cargar otras escenas
	CallbackClickeable cosa = [this]() {
		map.navigate("Artemisa");
		sdlutils().clearRenderer();
		map.renderBackGround();
		};
	clicker->addEvent(cosa);
	
	/*auto boxBg = addEntity();
	auto bgTr = boxBg->addComponent<Transform>(100, sdlutils().height() - 200, sdlutils().width()-200, 200);
	boxBg->addComponent<RenderImage>(&sdlutils().images().at("placeHolder"));

	auto dialogoBox = addEntity();
	auto textTr = dialogoBox->addComponent<Transform>(20, 20,100,100);
	textTr->setParent(bgTr);
	dialogoBox->addComponent<RenderImage>();
	dialogoBox->addComponent<DialogComponent>(&dialogMngr_);*/


	/*Texture* texturaBoton = &sdlutils().images().at("press");
	Entity* BotonPress = addEntity();

	Transform* transformBoton = BotonPress->addComponent<Transform>(260.0f, 480.0f, texturaBoton->width(), texturaBoton->height());
	RenderImage* renderBoton = BotonPress->addComponent<RenderImage>(texturaBoton);

	auto clickerPress = BotonPress->addComponent<Clickeable>();
	Callback funcPress = [](Entity* a) {
		gm().changeScene(ecs::sc::EXPLORE_SCENE, ecs::sc::MAIN_SCENE);
	};
	clickerPress->add*ent(funcPress);*/





	// Para Dani: El Personaje PlaceHolder que te he creado se compone del bot�n de press que al pulsarse te crea
// la caja de fondo y te empieza a renderizar el texto (ojo: si lo pulsas varias veces creas varias, esto lo puedes 
// solucionar sacando las entidades de box al h y comprobando si punteros a entidad son null o con un booleano que
// haga de flag)

// Para Dani: Aqu� le hacemos clickable y le ponemos como callback el m�todo funcPress
	Entity* BotonPress = addEntity();
	Texture* texturaBoton = &sdlutils().images().at("press");
	Transform* transformBoton = BotonPress->addComponent<Transform>(260.0f, 480.0f, texturaBoton->width(), texturaBoton->height());
	RenderImage* renderBoton = BotonPress->addComponent<RenderImage>(texturaBoton);
	auto clickerPress = BotonPress->addComponent<Clickeable>();
	CallbackClickeable funcPress = [this]() {
		//Esto ser�a la caja del fondo (lo de SDL que se ve)
		auto boxBg = addEntity();
		auto bgTr = boxBg->addComponent<Transform>(100, sdlutils().height() - 200, sdlutils().width() - 200, 200);
		boxBg->addComponent<RenderImage>(&sdlutils().images().at("placeHolder"));

		//Aqu� pillar�a el di�logo con el manager y crear�a la entidad que lo renderiza
		auto dialogoBox = addEntity();
		auto textTr = dialogoBox->addComponent<Transform>(20, 20, 100, 100);
		textTr->setParent(bgTr);
		dialogoBox->addComponent<RenderImage>();
		dialogoBox->addComponent<DialogComponent>(&dialogMngr_);
		};
	clickerPress->addEvent(funcPress);

}

void ecs::ExplorationScene::render()
{
	map.renderBackGround();
	Scene::render();
}
