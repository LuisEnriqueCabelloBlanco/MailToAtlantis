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
	delete rightTex;
	delete leftTex;
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

	ecs::Entity* Arrow = factory_->createImageButton(pos, size, sujetaplazas, cosa, "click");

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

	// texto
	ecs::Entity* textoDiarioLeft = addEntity(ecs::layer::UI);
	leftPageTr = textoDiarioLeft->addComponent<Transform>(55, 80, 1, 1);
	leftPageTr->setParent(diario_->getComponent<Transform>());
	leftPageRnd = textoDiarioLeft->addComponent<RenderImage>();

	ecs::Entity* textoDiarioRight = addEntity(ecs::layer::UI);
	rightPageTr = textoDiarioRight->addComponent<Transform>(307, 40, 1, 1);
	rightPageTr->setParent(diario_->getComponent<Transform>());
	rightPageRnd = textoDiarioRight->addComponent<RenderImage>();

	//carita felicidad
	ecs::Entity* caraFel = addEntity(ecs::layer::UI);
	auto caraFelTr = caraFel->addComponent<Transform>(267, 28, 28, 28);
	caraFelTr->setParent(diario_->getComponent<Transform>());
	caraFelicidad = caraFel->addComponent<RenderImage>();
	caraFelicidad->setTexture(nullptr);

	setupDiarioPages();

	// botones de pasar pagina

	factory_->setLayer(ecs::layer::UI);
	ecs::Entity* pasarPagIzq = factory_->createImageButton(Vector2D(43,315),Vector2D(40,40), 
		&sdlutils().images().at("cambioPag"), [this]() {
			changeDiarioPages(false);
		});
	pasarPagIzq->getComponent<Transform>()->setParent(diario_->getComponent<Transform>());
	pasarPagIzq->getComponent<Transform>()->setFlip(SDL_FLIP_HORIZONTAL);

	ecs::Entity* pasarPagDer = factory_->createImageButton(Vector2D(516, 315), Vector2D(40, 40),
		&sdlutils().images().at("cambioPag"), [this]() {
			changeDiarioPages(true);
		});
	pasarPagDer->getComponent<Transform>()->setParent(diario_->getComponent<Transform>());

	factory_->setLayer(ecs::layer::DEFAULT);

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

	
}

void ecs::ExplorationScene::setupDiarioPages() {
	diarioText_.clear();
	pagesByCharacter = std::vector<int>(7, 0);
	RenderImage* rendComp = diario_->getComponent<RenderImage>();
	if (rendComp == nullptr)
		rendComp = diario_->addComponent<RenderImage>();

	std::vector<Texture*> textureVec;
	int firstPersonaje = -1;
	bool diarioVacio = true;
	for (int i = 0; i < 7; i++) {
		NPCdata* data = generalData().getNPCData((npc::Personaje)i);
		if (data->felicidad != NoHabladoAun)
		{
			diarioVacio = false;
			//procesamos los textos
			std::string textoPersonaje = "";
			int j = 0;
			bool eventoCompletado = true;
			while (eventoCompletado && j < data->eventosCompletados.size()) {
				eventoCompletado = data->eventosCompletados[j].first;
				if (eventoCompletado) {
					std::string textoCompletado = " (EN CURSO)";
					textoPersonaje = textoPersonaje + "- Dia ";
					if (data->eventosCompletados[j].second == 0) // si el evento es de hoy
					{
						textoPersonaje = textoPersonaje + std::to_string(generalData().getDay()) +
							textoCompletado + "\n" + 
							data->events[(generalData().getDay() - 1)]->textoDiario + "\n";
					}
					else
					{
						if (data->eventosCompletados[j].second > 0)
							textoCompletado = " (COMPLETADO)";
						else
							textoCompletado = " (FALLIDO)";

						textoPersonaje = textoPersonaje + std::to_string(std::abs(data->eventosCompletados[j].second))
							+ "- " + textoCompletado + "\n"
							+ data->events[abs(data->eventosCompletados[j].second) - 1]->textoDiario + "\n";
					}
				}
				j++;
			}

			

			j = 0;
			while (textoPersonaje.size() > 0) {
				int maxLen = j % 2 == 0 ? MAX_CHAR_LEN_LEFT_DIARIO : MAX_CHAR_LEN_RIGHT_DIARIO;
				std::string provisionalSubstring = textoPersonaje.substr(0, maxLen);
				int numSaltosLinea = 0;
				for (int i = 0; i < provisionalSubstring.size(); i++) {
					if (provisionalSubstring[i] == '\n')
						numSaltosLinea++;
				}
				maxLen = maxLen - (numSaltosLinea * 13);

				diarioText_.push_back(textoPersonaje.substr(0, maxLen));
				if (textoPersonaje.size() < maxLen)
					textoPersonaje.clear();
				else
					textoPersonaje = textoPersonaje.substr(maxLen);
				j++;
			}

			pagesByCharacter[i] = j;
			// añadimos pag vacia para que no quede desparejo
			if (j % 2 != 0) {
				diarioText_.push_back(" ");
				pagesByCharacter[i]++;
			}
				
			if (firstPersonaje == -1)
				firstPersonaje = i;

			//ponemos sus paginas
			if (j == 0) // tiene que haber una aunque no haya texto
			{
				textureVec.push_back(&sdlutils().images().at("diario" + std::to_string(i + 1)));
				diarioText_.push_back(" ");
				diarioText_.push_back(" ");
			}
			else
			{
				for (int k = 0; k < j; k++) {
					if (k % 2 == 0)
						textureVec.push_back(&sdlutils().images().at("diario" + std::to_string(i + 1)));
				}
			}
		}
	}

	if (diarioVacio)
		textureVec.push_back(&sdlutils().images().at("bookTest"));

	diario_->getComponent<RenderImage>()->getVector()->clear();
	diario_->getComponent<RenderImage>()->setVector(textureVec);

	if (!diarioVacio)
	{
		delete rightPageRnd->getTexture();
		rightPageRnd->setVector(std::vector<Texture*>(1, nullptr));
		delete leftPageRnd->getTexture();		
		leftPageRnd->setVector(std::vector<Texture*>(1, nullptr));

		currentDiarioPage = 0;
		leftTex = new Texture(sdlutils().renderer(),
			diarioText_.size() < 1 ? " " : diarioText_[currentDiarioPage],
			sdlutils().fonts().at("simpleHandmade20"),
			build_sdlcolor(0x00000000ff), 245);
		leftPageRnd->setTexture(leftTex);
		leftPageTr->setWidth(leftPageRnd->getTexture()->width());
		leftPageTr->setHeith(leftPageRnd->getTexture()->height());
		 rightTex = new Texture(sdlutils().renderer(),
			diarioText_.size() < 1 ? " " : diarioText_[currentDiarioPage + 1],
			sdlutils().fonts().at("simpleHandmade20"),
			build_sdlcolor(0x00000000ff), 245);
		rightPageRnd->setTexture(rightTex);
		rightPageTr->setWidth(rightPageRnd->getTexture()->width());
		rightPageTr->setHeith(rightPageRnd->getTexture()->height());
	}

	if (firstPersonaje == -1)
		caraFelicidad->setTexture(nullptr);
	else
		changeCaraFelicidad(generalData().getNPCData((Personaje)firstPersonaje));
}

void ecs::ExplorationScene::changeDiarioPages(bool forward) {
	if (forward) {
		diario_->getComponent<RenderImage>()->nextTexture();
		if (currentDiarioPage < diarioText_.size() - 2)
			currentDiarioPage = currentDiarioPage + 2;
	}
	else {
		diario_->getComponent<RenderImage>()->previousTexture();
		if (currentDiarioPage > 0)
			currentDiarioPage = currentDiarioPage - 2;
	}
	
	int i = 0;
	bool texFound = false;
	Texture* tex = diario_->getComponent<RenderImage>()->getTexture();
	while (!texFound && i < 7) {
		texFound = tex == &sdlutils().images().at("diario" + std::to_string(i + 1));
		i++;
	}
	if (texFound)
		changeCaraFelicidad(generalData().getNPCData((Personaje)(i - 1)));

	delete rightPageRnd->getTexture();
	rightPageRnd->setVector(std::vector<Texture*>(1, nullptr));
	delete leftPageRnd->getTexture();
	leftPageRnd->setVector(std::vector<Texture*>(1, nullptr));
	
	leftTex = new Texture(sdlutils().renderer(),
		diarioText_.size() < 1 ? " " : diarioText_[currentDiarioPage],
		sdlutils().fonts().at("simpleHandmade20"),
		build_sdlcolor(0x00000000ff), 245);
	leftPageRnd->setTexture(leftTex);
	leftPageTr->setWidth(leftPageRnd->getTexture()->width());
	leftPageTr->setHeith(leftPageRnd->getTexture()->height());
	rightTex = new Texture(sdlutils().renderer(),
		diarioText_.size() < 1 ? " " : diarioText_[currentDiarioPage + 1],
		sdlutils().fonts().at("simpleHandmade20"),
		build_sdlcolor(0x00000000ff), 245);
	rightPageRnd->setTexture(rightTex);
	rightPageTr->setWidth(rightPageRnd->getTexture()->width());
	rightPageTr->setHeith(rightPageRnd->getTexture()->height());
}

void ecs::ExplorationScene::changeCaraFelicidad(NPCdata* data) {
	if (caraFelicidad->getTexture() != nullptr)
		caraFelicidad->getVector()->clear();
	caraFelicidad->setVector(std::vector<Texture*>(1, nullptr));
	switch (data->felicidad) {
	case Minima:
		caraFelicidad->setTexture(&sdlutils().images().at("caraFelicidadMinimo"));
		break;
	case Mala:
		caraFelicidad->setTexture(&sdlutils().images().at("caraFelicidadMalo"));
		break;
	case Normal:
		caraFelicidad->setTexture(&sdlutils().images().at("caraFelicidadNormal"));
		break;
	case Buena:
		caraFelicidad->setTexture(&sdlutils().images().at("caraFelicidadBueno"));
		break;
	case Maxima:
		caraFelicidad->setTexture(&sdlutils().images().at("caraFelicidadMaxima"));
		break;
	}
}

void ecs::ExplorationScene::addDiarioEvent(NPCevent* event)
{
	NPCdata* data = generalData().getNPCData(event->personaje);
	int i = 0;
	while (i < data->eventosCompletados.size())
	{
		if (!data->eventosCompletados[i].first)
			break;
		i++;
	}
	generalData().getNPCData(event->personaje)->eventosCompletados[i] = std::make_pair(true, 0);
	setupDiarioPages();
}

ecs::Entity* ecs::ExplorationScene::createCharacter(Vector2D pos, const std::string& character, float scale) {

	ComonObjectsFactory factory(this);

	Texture* characterTexture = &sdlutils().images().at(character);
	Vector2D size{ characterTexture->width() * scale, characterTexture->height() * scale };
	
	//QA: DETECTAR CUANTAS VECES SE HA PULSADO EN CADA PERSONAJE EN LA FASE DE EXPLORACION
	//Actualmente los personajes no tienen memoria, si queremos esto haria falta anadrile un parametro

	// al pulsar sale el dialogo, el dialogue manager y el dialogue component se encargan de todo, no me direis que esto no es mas sencillo de usar que todo lo que habia que hacer antes jajajaj
	CallbackClickeable funcPress = [this, character]() {
		if (generalData().getNPCData(generalData().stringToPersonaje(character))->felicidad == npc::Maxima) {
			generalData().unlockUpgrade(generalData().stringToPersonaje(character));
		}
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
				addDiarioEvent(event);
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

	ecs::Entity* objEnt = factory_->createImageButton(pos, size, nullptr, funcPress, "click");

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
		if (generalData().getNPCData(generalData().stringToPersonaje(characters[i].name_))->felicidad != npc::SeFue) {
			lugares[placeName].addObjects(createCharacter(characters[i].pos,
				characters[i].name_, characters[i].scale_));
		}
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
			}, "click", SDL_Color{255,255,0});
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


