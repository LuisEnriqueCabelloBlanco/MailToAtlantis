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
#include "../architecture/Config.h"
#include "../architecture/GeneralData.h"
#include <string>
#include "../sdlutils/Texture.h"
#include "../components/DialogComponent.h"
#include "../sistemas/ComonObjectsFactory.h"
#include "../architecture/GeneralData.h"
#include "../components/DelayedCallback.h"
#include <architecture/GameConstants.h>
#include <QATools/DataCollector.h>
ecs::ExplorationScene::ExplorationScene() :Scene()
{

	generalData().setDayData();
	initPlacesDefaultMap();
	initDirectionsDefaultMap();
	rect_ = build_sdlrect(0, 0, LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH);
	canStartConversation = true;
}

ecs::ExplorationScene::~ExplorationScene()
{

}

void ecs::ExplorationScene::init()
{
	std::cout << "Hola Exploracion" << std::endl;
	//setNavegabilityOfPlace("Hermes"); // Esto es para probar si funciona el seteo.

	generalData().setDayData();
	generalData().updateDia();
	updateNavegavility();

	for (auto& e : objs_) {
		for (auto en : e){
				en->setAlive(false);

		}
	}
	actualPlace_ = &lugares[generalData().fromDistritoToString(pq::Distrito::Hestia)];

	createObjects(pq::Distrito::Hestia);

	

}


void ecs::ExplorationScene::initPlacesDefaultMap()
{
	
	for (int i = 0; i < generalData().getNumDistritos(); ++i) {

		std::string placeName = generalData().fromDistritoToString(i);

		Lugar aux = Lugar(&sdlutils().images().at(placeName), false);

		lugares.insert({ placeName, aux });

	}
	
}

void ecs::ExplorationScene::initDirectionsDefaultMap()
{
	//Hestia
	lugares[generalData().fromDistritoToString(pq::Distrito::Hestia)].addDirections("Hefesto", &lugares[generalData().fromDistritoToString(pq::Distrito::Hefesto)]);
	lugares[generalData().fromDistritoToString(pq::Distrito::Hestia)].addDirections("Artemisa", &lugares[generalData().fromDistritoToString(pq::Distrito::Artemisa)]);

	//Artemisa
	lugares[generalData().fromDistritoToString(pq::Distrito::Artemisa)].addDirections("Demeter", &lugares[generalData().fromDistritoToString(pq::Distrito::Demeter)]);
	lugares[generalData().fromDistritoToString(pq::Distrito::Artemisa)].addDirections("Hestia", &lugares[generalData().fromDistritoToString(pq::Distrito::Hestia)]);

	//Demeter
	lugares[generalData().fromDistritoToString(pq::Distrito::Demeter)].addDirections("Hermes", &lugares[generalData().fromDistritoToString(pq::Distrito::Hermes)]);
	lugares[generalData().fromDistritoToString(pq::Distrito::Demeter)].addDirections("Hefesto", &lugares[generalData().fromDistritoToString(pq::Distrito::Hefesto)]);
	lugares[generalData().fromDistritoToString(pq::Distrito::Demeter)].addDirections("Artemisa", &lugares[generalData().fromDistritoToString(pq::Distrito::Artemisa)]);

	//Hefesto
	lugares[generalData().fromDistritoToString(pq::Distrito::Hefesto)].addDirections("Demeter", &lugares[generalData().fromDistritoToString(pq::Distrito::Demeter)]);
	lugares[generalData().fromDistritoToString(pq::Distrito::Hefesto)].addDirections("Hestia", &lugares[generalData().fromDistritoToString(pq::Distrito::Hestia)]);
	lugares[generalData().fromDistritoToString(pq::Distrito::Hefesto)].addDirections("Hermes", &lugares[generalData().fromDistritoToString(pq::Distrito::Hermes)]);

	//Hermes
	lugares[generalData().fromDistritoToString(pq::Distrito::Hermes)].addDirections("Demeter", &lugares[generalData().fromDistritoToString(pq::Distrito::Demeter)]);
	lugares[generalData().fromDistritoToString(pq::Distrito::Hermes)].addDirections("Hefesto", &lugares[generalData().fromDistritoToString(pq::Distrito::Hefesto)]);
	lugares[generalData().fromDistritoToString(pq::Distrito::Hermes)].addDirections("Apolo", &lugares[generalData().fromDistritoToString(pq::Distrito::Apolo)]);

	//Apolo
	lugares[generalData().fromDistritoToString(pq::Distrito::Apolo)].addDirections("Hermes", &lugares[generalData().fromDistritoToString(pq::Distrito::Hermes)]);
	lugares[generalData().fromDistritoToString(pq::Distrito::Apolo)].addDirections("Poseidon", &lugares[generalData().fromDistritoToString(pq::Distrito::Poseidon)]);

	//Poseidon
	lugares[generalData().fromDistritoToString(pq::Distrito::Poseidon)].addDirections("Apolo", &lugares[generalData().fromDistritoToString(pq::Distrito::Apolo)]);
}

void ecs::ExplorationScene::render()
{

	actualPlace_->getTexture()->render(rect_);
	Scene::render();

#ifdef DEV_

#endif // DEV_

}

void ecs::ExplorationScene::update() {
	Scene::update();

	if (placeToGo >= 0 && placeToGo < generalData().getNumDistritos()) {

		navigate(generalData().fromDistritoToString(placeToGo));
		createObjects(placeToGo);
		placeToGo = -1;		
		
	}
	
}

void ecs::ExplorationScene::navigate(std::string placeDir) // otro string sin const
{

	//QA: ALMACENAR EN ORDEN LOS LUGARES QUE HA RECORRIDO EN CADA FASE DE EXPLORACION EL JUGADOR
	//QA: ALMACENAR CUANTO TIEMPO SE QUEDA CADA JUGADOR EN UN LUGAR DEL MAPA
	
	if (actualPlace_->navigate(placeDir)) {
		actualPlace_ = actualPlace_->getPlaceFromDirection(placeDir);

#ifdef QA_TOOLS
		dataCollector().recordNavigation(placeDir);
#endif // QA_TOOLS

	}

	
}

ecs::Entity* ecs::ExplorationScene::createNavegationsArrows(Vector2D pos, std::string place, float scale, int flip)
{
	//para crear la flecha a hefesto

	ComonObjectsFactory factory(this);
	factory.setLayer(ecs::layer::FOREGROUND);
	Texture* sujetaplazas;

	int placeID = generalData().fromStringToDistrito(place);

	if(placeID < lugares.size() && lugares[place].isNavegable())
		sujetaplazas = &sdlutils().images().at("cartel" + place);
	else
		sujetaplazas = &sdlutils().images().at("cruz");

	Vector2D size{ sujetaplazas->width() * scale, sujetaplazas->height() * scale };
	
	CallbackClickeable cosa = [this, place, placeID]() {
		if (actualPlace_->navigate(place)) {
			closeConversation();
			actualPlace_->changeActivationObjects(false);
			placeToGo = placeID;
			
		}
	};

	ecs::Entity* Arrow = factory.createImageButton(pos, size, sujetaplazas, cosa);

	Transform* arrowTR = Arrow->getComponent<Transform>();

	if (flip == 1) {
		arrowTR->setFlip(SDL_FLIP_VERTICAL);
	}
	else if (flip == 2) {
		arrowTR->setFlip(SDL_FLIP_HORIZONTAL);
	}
	else {
		arrowTR->setFlip(SDL_FLIP_NONE);
	}

	factory.setLayer(ecs::layer::DEFAULT);
	
	return Arrow;

}

ecs::Entity* ecs::ExplorationScene::createWorkButton(Vector2D pos, Vector2D scale) {

	ecs::Entity* e = addEntity();
	e->addComponent<Transform>(pos.getX(), pos.getY(), scale.getX(), scale.getY());
	auto clickableBotonTrabajar = e->addComponent<Clickeable>();
	CallbackClickeable funcPress = [this]() {
		gm().requestChangeScene(ecs::sc::EXPLORE_SCENE, ecs::sc::MAIN_SCENE);
	};
	clickableBotonTrabajar->addEvent(funcPress);


	return e;
}

ecs::Entity* ecs::ExplorationScene::createCharacter(Vector2D pos, const std::string& character, float scale) {

	ComonObjectsFactory factory(this);

	Texture* texturaBoton = &sdlutils().images().at(character);
	Vector2D size{ texturaBoton->width() * scale, texturaBoton->height() * scale };
	
	//QA: DETECTAR CUANTAS VECES SE HA PULSADO EN CADA PERSONAJE EN LA FASE DE EXPLORACION
	//Actualmente los personajes no tienen memoria, si queremos esto har�a falta a�adrile un parametro

	// al pulsar sale el dialogo
	CallbackClickeable funcPress = [this, character]() {

		if (canStartConversation)
		{
			auto charac = generalData().stringToPersonaje(character);
			auto data = generalData().getNPCData(charac);
			canStartConversation = false;

			boxBackground->getComponent<RenderImage>()->setTexture(&sdlutils().images().at("cuadroDialogo"));
			// activamos los dialogos correspondientes
			std::pair<const std::string, int> aux = data->getDialogueInfo();

			dialogMngr_.setDialogues((DialogManager::DialogSelection)generalData().stringToPersonaje(character), aux.first, aux.second);

			textDialogue->addComponent<DialogComponent>(&dialogMngr_, this);

			dataCollector().recordNPC(charac +1,aux.second, generalData().getNPCData(charac)->felicidad);
		}
	};

	ecs::Entity* BotonPress = factory.createImageButton(pos, size, texturaBoton, funcPress);
	
	return BotonPress;
}

void ecs::ExplorationScene::setNavegabilityOfPlace(int place, bool value)
{
	if(place < lugares.size())
	{
		lugares[generalData().fromDistritoToString(place)].setNavegability(value);
	}
}

void ecs::ExplorationScene::updateNavegavility()
{
	for (std::string g : generalData().getPlacesToActive())
		setNavegabilityOfPlace(generalData().fromStringToDistrito(g));
}

void ecs::ExplorationScene::createObjects(int place) {

	auto& pl = config().places();

	std::string placeName = generalData().fromDistritoToString(place);

	for (int i = 0; i < pl.at(placeName).myArrows.size(); ++i) {

		lugares[generalData().fromDistritoToString(place)].addObjects(createNavegationsArrows(pl.at(placeName).myArrows[i].pos,
			pl.at(placeName).myArrows[i].destination_, pl.at(placeName).myArrows[i].scale_, pl.at(placeName).myArrows[i].flip_));


	}

	for (int i = 0; i < pl.at(placeName).myCharacters.size(); ++i) {

		lugares[generalData().fromDistritoToString(place)].addObjects(createCharacter(pl.at(placeName).myCharacters[i].pos,
			pl.at(placeName).myCharacters[i].name_, pl.at(placeName).myCharacters[i].scale_));


	}

	if (place == pq::Distrito::Hestia) {
		//boton ir a trabajar
		boton_Trabajo = createWorkButton({ 650, 400 }, { 100, 300 });

		lugares[generalData().fromDistritoToString(place)].addObjects(boton_Trabajo);
	}


	// creamos la entidad caja dialogo
	boxBackground = addEntity(ecs::layer::UI);
	auto bgTr = boxBackground->addComponent<Transform>(100, LOGICAL_RENDER_HEITH - 250, LOGICAL_RENDER_WIDTH - 100, 200);
	boxBackground->addComponent<RenderImage>(nullptr);

	// entidad del texto
	textDialogue = addEntity(ecs::layer::UI);
	auto textTr = textDialogue->addComponent<Transform>(100, 40, 80, 100);
	textTr->setParent(bgTr);
	textDialogue->addComponent<RenderImage>();
}

void ecs::ExplorationScene::closeConversation() {
	textDialogue->getComponent<RenderImage>()->setTexture(nullptr);
	textDialogue->removeComponent<DialogComponent>();
	boxBackground->getComponent<RenderImage>()->setTexture(nullptr);
	textDialogue->addComponent<DelayedCallback>(0.1, [this]() {
		canStartConversation = true;
		});
}


//LUGAR__________________________________________________________________________________________

void ecs::Lugar::addDirections(std::string placeDir, Lugar* place)
{
	directions_[placeDir] = place;
}

bool ecs::Lugar::navigate(std::string placeDir)
{
	return directions_.count(placeDir) && directions_.at(placeDir)->navegable_;
}

ecs::Lugar* ecs::Lugar::getPlaceFromDirection(std::string placeDir)
{
	return directions_[placeDir];
}

void ecs::Lugar::changeActivationObjects(bool state)
{
	for (auto& e : ents_) {
		e->setAlive(state);
	}

}

void ecs::Lugar::addObjects(ecs::Entity* e)
{
	ents_.push_back(e);
}

bool ecs::Lugar::isNavegable() const
{
	return navegable_;
}

void ecs::Lugar::setNavegability(bool value)
{
	navegable_ = value;
}


