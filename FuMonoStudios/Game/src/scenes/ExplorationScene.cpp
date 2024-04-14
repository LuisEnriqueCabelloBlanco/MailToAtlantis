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
ecs::ExplorationScene::ExplorationScene() :Scene(), numLugares(7)
{

	generalData().setDayData();
	initPlacesDefaultMap();
	generalData().updateDia();
	updateNavegavility();
	initDirectionsDefaultMap();
	rect_ = build_sdlrect(0, 0, LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH);
	dialogMngr_.canStartConversation = true;
}

ecs::ExplorationScene::~ExplorationScene()
{

}

void ecs::ExplorationScene::init()
{
	std::cout << "Hola Exploracion" << std::endl;
	//setNavegabilityOfPlace("Hermes"); // Esto es para probar si funciona el seteo.
	generalData().updateDia();
	updateNavegavility();

	for (auto& e : objs_) {
		for (auto en : e){
				en->setAlive(false);

		}
	}
	actualPlace_ = &lugares[pq::Distrito::Hestia];
	createPlaces();

	lugares[pq::Distrito::Hestia].changeActivationObjects(true);

	//boton ir a trabajar
	boton_Trabajo = createWorkButton({ 650, 400 }, { 100, 300 });


	dialogMngr_.init(this);
}


void ecs::ExplorationScene::initPlacesDefaultMap()
{
	
	for (int i = 0; i < numLugares; ++i) {

		Lugar aux = Lugar(&sdlutils().images().at(generalData().fromDistritoToString(i)), false);

		lugares.emplace_back(aux);

	}
	
}

void ecs::ExplorationScene::initDirectionsDefaultMap()
{
	//Hestia
	lugares[pq::Distrito::Hestia].addDirections("Hefesto", &lugares[pq::Distrito::Hefesto]);
	lugares[pq::Distrito::Hestia].addDirections("Artemisa", &lugares[pq::Distrito::Artemisa]);

	//Artemisa
	lugares[pq::Distrito::Artemisa].addDirections("Demeter", &lugares[pq::Distrito::Demeter]);
	lugares[pq::Distrito::Artemisa].addDirections("Hestia", &lugares[pq::Distrito::Hestia]);

	//Demeter
	lugares[pq::Distrito::Demeter].addDirections("Hermes", &lugares[pq::Distrito::Hermes]);
	lugares[pq::Distrito::Demeter].addDirections("Hefesto", &lugares[pq::Distrito::Hefesto]);
	lugares[pq::Distrito::Demeter].addDirections("Artemisa", &lugares[pq::Distrito::Artemisa]);

	//Hefesto
	lugares[pq::Distrito::Hefesto].addDirections("Demeter", &lugares[pq::Distrito::Demeter]);
	lugares[pq::Distrito::Hefesto].addDirections("Hestia", &lugares[pq::Distrito::Hestia]);
	lugares[pq::Distrito::Hefesto].addDirections("Hermes", &lugares[pq::Distrito::Hermes]);	

	//Hermes
	lugares[pq::Distrito::Hermes].addDirections("Demeter", &lugares[pq::Distrito::Demeter]);
	lugares[pq::Distrito::Hermes].addDirections("Hefesto", &lugares[pq::Distrito::Hefesto]);
	lugares[pq::Distrito::Hermes].addDirections("Apolo", &lugares[pq::Distrito::Apolo]);

	//Apolo
	lugares[pq::Distrito::Apolo].addDirections("Hermes", &lugares[pq::Distrito::Hermes]);
	lugares[pq::Distrito::Apolo].addDirections("Poseidon", &lugares[pq::Distrito::Poseidon]);

	//Poseidon
	lugares[pq::Distrito::Poseidon].addDirections("Apolo", &lugares[pq::Distrito::Apolo]);
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

	if (!placeToGo.empty()) {

		int size = placeToGo.size();

		for (int i = 0; i < size; ++i) {
			navigate(generalData().fromDistritoToString(placeToGo[i]));
			lugares[placeToGo[i]].changeActivationObjects(true);
			placeToGo.clear();
		}
		
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

	
	if (placeDir != "Hestia") {

		if (boton_Trabajo->isActive()) {
			boton_Trabajo->setActive(false);
		}
		

	}
	else {

		if (!boton_Trabajo->isActive()) {
			boton_Trabajo->setActive(true);
		}
		

	}
	
	
}

ecs::Entity* ecs::ExplorationScene::createNavegationsArrows(Vector2D pos, std::string place, float scale, int flip)
{
	//para crear la flecha a hefesto

	ComonObjectsFactory factory(this);
	factory.setLayer(ecs::layer::FOREGROUND);
	Texture* sujetaplazas;

	int placeID = generalData().fromStringToDistrito(place);

	if(placeID < lugares.size() && lugares[placeID].isNavegable())
		sujetaplazas = &sdlutils().images().at("cartel" + place);
	else
		sujetaplazas = &sdlutils().images().at("cruz");

	Vector2D size{ sujetaplazas->width() * scale, sujetaplazas->height() * scale };
	
	CallbackClickeable cosa = [this, place, placeID]() {
		if (actualPlace_->navigate(place)) {
			dialogMngr_.closeConversation();
			actualPlace_->changeActivationObjects(false);
			placeToGo.push_back(placeID);
			
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

		if (dialogMngr_.canStartConversation)
		{
			auto charac = generalData().stringToPersonaje(character);
			auto data = generalData().getNPCData(charac);
			dialogMngr_.canStartConversation = false;

		    // activamos los dialogos correspondientes
			std::pair<const std::string, int> aux = data->getDialogueInfo();

			dialogMngr_.setDialogues((DialogManager::DialogSelection)generalData().stringToPersonaje(character), aux.first, aux.second);

			dialogMngr_.textDialogue->addComponent<DialogComponent>(&dialogMngr_);

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
		lugares[place].setNavegability();
	}
}

void ecs::ExplorationScene::updateNavegavility()
{
	for (std::string g : generalData().getPlacesToActive())
		setNavegabilityOfPlace(generalData().fromStringToDistrito(g));
}

void ecs::ExplorationScene::createPlaces() {
	int prueba = 0;

	for (int i = 0; i < numLugares; ++i) {
		createObjects(i);
	}

}

void ecs::ExplorationScene::createObjects(int place) {

	auto& pl = config().places();

	std::string placeName = generalData().fromDistritoToString(place);

	for (int i = 0; i < pl.at(placeName).myArrows.size(); ++i) {

		lugares[place].addObjects(createNavegationsArrows(pl.at(placeName).myArrows[i].pos,
			pl.at(placeName).myArrows[i].destination_, pl.at(placeName).myArrows[i].scale_, pl.at(placeName).myArrows[i].flip_));


	}

	for (int i = 0; i < pl.at(placeName).myCharacters.size(); ++i) {

		lugares[place].addObjects(createCharacter(pl.at(placeName).myCharacters[i].pos,
			pl.at(placeName).myCharacters[i].name_, pl.at(placeName).myCharacters[i].scale_));


	}

	lugares[place].changeActivationObjects(false);

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
		//canStartConversation = true;
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
		e->setActive(state);
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


