#include <utils/checkML.h>
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

#ifdef DEV_TOOLS
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#endif // DEV_TOOLS

#include "../sistemas/NPCeventSystem.h"
#include "../components/HoverSensorComponent.h"
#include "../components/MoverTransform.h"
#include <components/RenderWithLight.h>

ecs::ExplorationScene::ExplorationScene() :Scene()
{

}

ecs::ExplorationScene::~ExplorationScene()
{

}

void ecs::ExplorationScene::init()
{
	generalData().readIntObjData();
	rect_ = build_sdlrect(0, 0, LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH);
	canStartConversation = true;
#ifdef _DEBUG
	std::cout << "Hola Exploracion" << std::endl;
#endif // _DEBUG


	initPlacesDefaultMap();
	generalData().updateDia();
	updateNavegavility();
	initDirectionsDefaultMap();

	//for (auto& e : objs_) {
	//	for (auto en : e){
	//			en->setAlive(false);

	//	}
	//}
	clearScene();
	actualPlace_ = &lugares[generalData().fromDistritoToString(pq::Distrito::Hestia)];

	createObjects(pq::Distrito::Hestia);

	dialogMngr_.init(this);

	createDiario();
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

#ifdef DEV_TOOLS
	ImGui::NewFrame();
	makeDataWindow();
	ImGui::Render();

	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
#endif // DEV_

}

void ecs::ExplorationScene::update() {
	Scene::update();

	if (placeToGo >= 0 && placeToGo < generalData().getNumDistritos()) {

		navigate(generalData().fromDistritoToString(placeToGo));
		createObjects(placeToGo);
		placeToGo = -1;		
		
	}
	dialogMngr_.update();
}

void ecs::ExplorationScene::close() {
	clearScene();
	diario_->setAlive(false);
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

void ecs::ExplorationScene::makeDataWindow()
{
#ifdef DEV_TOOLS
	ImGui::Begin("Exploration Scene Data");
	if (ImGui::CollapsingHeader("Felicidad Npc")) {
		for (int i = 0; i < 7; i++) {
			auto npc = generalData().getNPCData((Personaje)i);
			std::string npcData = generalData().personajeToString((Personaje)i) + ": " + 
				npc::happinessToString.at(npc->felicidad);
			ImGui::Text(+ npcData.c_str());
		}
	}
	ImGui::End();
#endif // DEV_TOOLS
}

ecs::Entity* ecs::ExplorationScene::createNavegationsArrows(Vector2D pos, std::string place, float scale, int flip)
{
	//para crear la flecha a hefesto
	factory_->setLayer(ecs::layer::FOREGROUND);
	Texture* sujetaplazas;

	int placeID = generalData().fromStringToDistrito(place);

	if(placeID < lugares.size() && lugares[place].isNavegable())
		sujetaplazas = &sdlutils().images().at("cartel" + place);
	else
		sujetaplazas = &sdlutils().images().at("cruz");

	Vector2D size{ sujetaplazas->width() * scale, sujetaplazas->height() * scale };
	
	CallbackClickeable cosa = [this, place, placeID]() {
		if (actualPlace_->navigate(place)) {
			dialogMngr_.closeDialogue();
			actualPlace_->changeActivationObjects(false);
			placeToGo = placeID;
		}
	};

	ecs::Entity* Arrow = factory_->createImageButton(pos, size, sujetaplazas, cosa);

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

	factory_->setLayer(ecs::layer::DEFAULT);
	factory_->addHoverColorMod(Arrow, build_sdlcolor(0xaaaaaaff));
	return Arrow;

}

ecs::Entity* ecs::ExplorationScene::createWorkButton(Vector2D pos, Vector2D scale) {

	ecs::Entity* e = addEntity();
	e->addComponent<Transform>(pos.getX(), pos.getY(), scale.getX(), scale.getY());
	auto clickableBotonTrabajar = e->addComponent<Clickeable>("");
	CallbackClickeable funcPress = [this]() {
		gm().requestChangeScene(ecs::sc::EXPLORE_SCENE, ecs::sc::MAIN_SCENE);
	};
	clickableBotonTrabajar->addEvent(funcPress);
	return e;
}

void ecs::ExplorationScene::createDiario() {
	diario_ = addEntity(ecs::layer::UI);
	diario_->addComponent<Transform>(1300, 1000, 600, 400);
	diario_->addComponent<RenderImage>(&sdlutils().images().at("diario1"));
	diario_->addComponent<MoverTransform>(Easing::EaseOutBack);
	HoverSensorComponent* hoverComp = diario_->addComponent<HoverSensorComponent>();
	hoverComp->addInCall([this]() {
		MoverTransform* comp = diario_->getComponent<MoverTransform>();
		comp->setFinalPos(Vector2D(1300, 700));
		comp->setMoveTime(0.2);
		if (!comp->isEnabled())
			comp->enable();
	});
	hoverComp->addOutCall([this]() {
		MoverTransform* comp = diario_->getComponent<MoverTransform>();
		comp->setFinalPos(Vector2D(1300, 1000));
		comp->setMoveTime(0.2);
		if (!comp->isEnabled())
			comp->enable();
	});

	// texto
	ecs::Entity* textoDiario = addEntity(ecs::layer::UI);
	Transform* textTr = textoDiario->addComponent<Transform>(50, 50, 300, 600);
	textTr->setParent(diario_->getComponent<Transform>());
	RenderImage* textRnd = textoDiario->addComponent<RenderImage>();
	factory_->setFont("simpleHandmade");
	textRnd->setTexture(factory_->createTextTexture("Anemos", 50, build_sdlcolor(0x00000000ff), 20));
	factory_->setFont("arial");
}

ecs::Entity* ecs::ExplorationScene::createCharacter(Vector2D pos, const std::string& character, float scale) {

	ComonObjectsFactory factory(this);

	Texture* characterTexture = &sdlutils().images().at(character);
	Vector2D size{ characterTexture->width() * scale, characterTexture->height() * scale };
	
	//QA: DETECTAR CUANTAS VECES SE HA PULSADO EN CADA PERSONAJE EN LA FASE DE EXPLORACION
	//Actualmente los personajes no tienen memoria, si queremos esto haria falta anadrile un parametro

	// al pulsar sale el dialogo, el dialogue manager y el dialogue component se encargan de todo, no me direis que esto no es mas sencillo de usar que todo lo que habia que hacer antes jajajaj
	CallbackClickeable funcPress = [this, character]() {
	    dialogMngr_.startConversation(character);

		auto charac = generalData().stringToPersonaje(character); //de que personaje queremos el dialogo
		auto data = generalData().getNPCData(charac); //data de dicho personaje

		// activamos los dialogos correspondientes
		std::pair<const std::string, int> aux = data->getDialogueInfo();

		if (aux.first == "Eventos" || aux.first.substr(0, 3) == "Dia")
		{
			NPCevent* event = data->getEvent();
			if (event != nullptr)
			{
				for (int i = 0; i < event->numPaquetes; i++) {
					generalData().npcEventSys->addPaqueteNPC(event->paquetes[i]);
				}
				generalData().npcEventSys->activateEvent(event);
				generalData().npcEventSys->shuffleNPCqueue();
			}
		}
	};



	ecs::Entity* characterEnt = factory.createImageButton(pos, size, characterTexture, funcPress, "click");
	
	//return characterEnt;

	factory.addHoverColorMod(characterEnt, build_sdlcolor(0xccccccff));

	return characterEnt;
}

ecs::Entity* ecs::ExplorationScene::createInteractableObj(Vector2D pos, const std::string& interactableObj, float scaleX, float scaleY) {

	//Texture* characterTexture = &sdlutils().images().at(interactableObj);
	Vector2D size{ scaleX, scaleY };

	// al pulsar sale el dialogo, el dialogue manager y el dialogue component se encargan de todo, no me direis que esto no es mas sencillo de usar que todo lo que habia que hacer antes jajajaj
	CallbackClickeable funcPress = [this, interactableObj]() {
		dialogMngr_.startConversationWithObj(interactableObj);
	};

	ecs::Entity* objEnt = factory_->createImageButton(pos, size, nullptr, funcPress);

	return objEnt;
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

	auto& arrows = pl.at(placeName).myArrows;
	for (int i = 0; i < pl.at(placeName).myArrows.size(); ++i) {
		lugares[placeName].addObjects(createNavegationsArrows(arrows[i].pos,
			arrows[i].destination_, arrows[i].scale_, arrows[i].flip_));
	}
	auto& characters = pl.at(placeName).myCharacters;
	for (int i = 0; i < pl.at(placeName).myCharacters.size(); ++i) {
		lugares[placeName].addObjects(createCharacter(characters[i].pos,
			characters[i].name_, characters[i].scale_));
	}

	auto& intObjs = pl.at(placeName).myInteractableObjs;
	for (int i = 0; i < pl.at(placeName).myInteractableObjs.size(); ++i) {
		lugares[placeName].addObjects(createInteractableObj(intObjs[i].pos,
			intObjs[i].name_, intObjs[i].scaleX_, intObjs[i].scaleY_));
	}

	if (place == pq::Distrito::Hestia) {
		//boton ir a trabajar
		boton_Trabajo = createWorkButton({ 650, 400 }, { 100, 300 });

		lugares[placeName].addObjects(boton_Trabajo);

		//PLACEHOLDER_BOTON_GUARDADO
		factory_->createTextuButton(Vector2D(100, 100), "GUARDAR PARTIDA", 40, [this]() {
			generalData().saveGame();
			}, SDL_Color{255,255,0});
	}
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


