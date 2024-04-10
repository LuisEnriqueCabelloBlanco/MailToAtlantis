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
	initPlacesDefaultMap();
	generalData().updateDia();
	updateNavegavility();
	initDirectionsDefaultMap();
	actualPlace_ = &hestia;
	createObjects("Hestia");
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
	generalData().updateDia();
	updateNavegavility();

	for (auto& e : objs_) {
		for (auto en : e){
				en->setAlive(false);

		}
	}
	actualPlace_ = &hestia;
	createObjects("Hestia");

	//boton ir a trabajar
	boton_Trabajo = addEntity();
	boton_Trabajo->addComponent<Transform>(650, 400, 100, 300);
	auto clickableBotonTrabajar = boton_Trabajo->addComponent<Clickeable>();
	CallbackClickeable funcPress = [this]() {
		gm().requestChangeScene(ecs::sc::EXPLORE_SCENE, ecs::sc::MAIN_SCENE);
	};
	clickableBotonTrabajar->addEvent(funcPress);

}


void ecs::ExplorationScene::initPlacesDefaultMap()
{
	//Hestia
	hestia = Lugar(&sdlutils().images().at("hestia"), false);
	places["Hestia"] = &hestia;

	//Hefesto
	hefesto = Lugar(&sdlutils().images().at("hefesto"), false);
	places["Hefesto"] = &hefesto;

	//Demeter
	demeter = Lugar(&sdlutils().images().at("demeter"), false);
	places["Demeter"] = &demeter;

	//Artemisa
	artemisa = Lugar(&sdlutils().images().at("artemisa"), false);
	places["Artemisa"] = &artemisa;

	//Hermes
	hermes = Lugar(&sdlutils().images().at("hermes"), false);
	places["Hermes"] = &hermes;

	//Apolo
	apolo = Lugar(&sdlutils().images().at("apolo"), false);
	places["Apolo"] = &apolo;

	//Posidon
	poseidon = Lugar(&sdlutils().images().at("poseidon"), false);
	places["Poseidon"] = &poseidon;
}

void ecs::ExplorationScene::initDirectionsDefaultMap()
{
	//Hestia
	hestia.addDirections("Hefesto", &hefesto);
	hestia.addDirections("Artemisa", &artemisa);
	//Hefesto
	hefesto.addDirections("Demeter", &demeter);
	hefesto.addDirections("Hestia", &hestia);
	hefesto.addDirections("Hermes", &hermes);

	//Demeter
	demeter.addDirections("Hermes", &hermes);
	demeter.addDirections("Hefesto", &hefesto);
	demeter.addDirections("Artemisa", &artemisa);

	//Artemisa
	artemisa.addDirections("Demeter", &demeter);
	artemisa.addDirections("Hestia", &hestia);

	//Hermes
	hermes.addDirections("Demeter", &demeter);
	hermes.addDirections("Hefesto", &hefesto);
	hermes.addDirections("Apolo", &apolo);

	//Apolo
	apolo.addDirections("Hermes", &hermes);
	//apolo.addDirections("Poseidon", &poseidon);
	//Poseidon
	poseidon.addDirections("Apolo", &apolo);
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
			navigate(placeToGo[i]);
			createObjects(placeToGo[i]);
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

ecs::Entity* ecs::ExplorationScene::createNavegationsArrows(Vector2D pos, std::string placeDir, float scale, int flip)
{
	//para crear la flecha a hefesto

	ComonObjectsFactory factory(this);
	factory.setLayer(ecs::layer::FOREGROUND);
	Texture* sujetaplazas;
	if(places.count(placeDir) && places.at(placeDir)->isNavegable())
		sujetaplazas = &sdlutils().images().at("cartel" + placeDir);
	else
		sujetaplazas = &sdlutils().images().at("cruz");

	Vector2D size{ sujetaplazas->width() * scale, sujetaplazas->height() * scale };
	
	CallbackClickeable cosa = [this, placeDir]() {
		if (actualPlace_->navigate(placeDir)) {
			closeConversation();
			actualPlace_->killObjects();
			placeToGo.push_back(placeDir);
			
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

void ecs::ExplorationScene::setNavegabilityOfPlace(std::string place, bool value)
{
	if(places.count(place))
	{
		places.at(place)->setNavegability();
	}
}

void ecs::ExplorationScene::updateNavegavility()
{
	for (std::string g : generalData().getPlacesToActive())
		setNavegabilityOfPlace(g);
}

void ecs::ExplorationScene::createObjects(std::string place) {

	auto& pl = config().places();

	if (place == "Demeter") {

		for (int i = 0; i < pl.at(place).myArrows.size(); ++i) {
			
			demeter.addObjects(createNavegationsArrows(pl.at(place).myArrows[i].pos,
				pl.at(place).myArrows[i].destination_, pl.at(place).myArrows[i].scale_, pl.at(place).myArrows[i].flip_));


		}

		for (int i = 0; i < pl.at(place).myCharacters.size(); ++i) {

			demeter.addObjects(createCharacter(pl.at(place).myCharacters[i].pos,
				pl.at(place).myCharacters[i].name_, pl.at(place).myCharacters[i].scale_));


		}
	}
	else if (place == "Hefesto")
	{
		for (int i = 0; i < pl.at(place).myArrows.size(); ++i) {

			hefesto.addObjects(createNavegationsArrows(pl.at(place).myArrows[i].pos,
				pl.at(place).myArrows[i].destination_, pl.at(place).myArrows[i].scale_, pl.at(place).myArrows[i].flip_));


		}

		for (int i = 0; i < pl.at(place).myCharacters.size(); ++i) {

			hefesto.addObjects(createCharacter(pl.at(place).myCharacters[i].pos,
				pl.at(place).myCharacters[i].name_, pl.at(place).myCharacters[i].scale_));


		}
	}
	else if (place == "Hestia") {
		for (int i = 0; i < pl.at(place).myArrows.size(); ++i) {
			hestia.addObjects(createNavegationsArrows(pl.at(place).myArrows[i].pos,
				pl.at(place).myArrows[i].destination_, pl.at(place).myArrows[i].scale_, pl.at(place).myArrows[i].flip_));

		}

		for (int i = 0; i < pl.at(place).myCharacters.size(); ++i) {

			hestia.addObjects(createCharacter(pl.at(place).myCharacters[i].pos,
				pl.at(place).myCharacters[i].name_, pl.at(place).myCharacters[i].scale_));


		}
		
	}
	else if (place == "Artemisa") {
		for (int i = 0; i < pl.at(place).myArrows.size(); ++i) {

			artemisa.addObjects(createNavegationsArrows(pl.at(place).myArrows[i].pos,
				pl.at(place).myArrows[i].destination_, pl.at(place).myArrows[i].scale_, pl.at(place).myArrows[i].flip_));


		}

		for (int i = 0; i < pl.at(place).myCharacters.size(); ++i) {

			artemisa.addObjects(createCharacter(pl.at(place).myCharacters[i].pos,
				pl.at(place).myCharacters[i].name_, pl.at(place).myCharacters[i].scale_));


		}
	}
	else if (place == "Hermes") {
		for (int i = 0; i < pl.at(place).myArrows.size(); ++i) {

			hermes.addObjects(createNavegationsArrows(pl.at(place).myArrows[i].pos,
				pl.at(place).myArrows[i].destination_, pl.at(place).myArrows[i].scale_, pl.at(place).myArrows[i].flip_));


		}

		for (int i = 0; i < pl.at(place).myCharacters.size(); ++i) {

			hermes.addObjects(createCharacter(pl.at(place).myCharacters[i].pos,
				pl.at(place).myCharacters[i].name_, pl.at(place).myCharacters[i].scale_));


		}
	}
	else if (place == "Apolo") {
		for (int i = 0; i < pl.at(place).myArrows.size(); ++i) {

			apolo.addObjects(createNavegationsArrows(pl.at(place).myArrows[i].pos,
				pl.at(place).myArrows[i].destination_, pl.at(place).myArrows[i].scale_, pl.at(place).myArrows[i].flip_));


		}

		for (int i = 0; i < pl.at(place).myCharacters.size(); ++i) {

			apolo.addObjects(createCharacter(pl.at(place).myCharacters[i].pos,
				pl.at(place).myCharacters[i].name_, pl.at(place).myCharacters[i].scale_));


		}
	}
	else if (place == "Poseidon") {
		for (int i = 0; i < pl.at(place).myArrows.size(); ++i) {

			poseidon.addObjects(createNavegationsArrows(pl.at(place).myArrows[i].pos,
				pl.at(place).myArrows[i].destination_, pl.at(place).myArrows[i].scale_, pl.at(place).myArrows[i].flip_));


		}

		for (int i = 0; i < pl.at(place).myCharacters.size(); ++i) {

			poseidon.addObjects(createCharacter(pl.at(place).myCharacters[i].pos,
				pl.at(place).myCharacters[i].name_, pl.at(place).myCharacters[i].scale_));


		}
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

void ecs::Lugar::killObjects()
{
	for (auto& e : ents_) {
		e->setAlive(false);
	}
	ents_.clear();
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


