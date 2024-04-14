#include "MainScene.h"
#include "../architecture/Entity.h"
#include <iostream>
#include <fstream>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"
#include "../components/Render.h"
#include "../components/Clickeable.h"
#include "../components/DragAndDrop.h"
#include "../components/Trigger.h"
#include <string>
#include <list>
#include "../sdlutils/Texture.h"
#include "../components/PackageChecker.h"
#include "../components/Gravity.h"
#include "../components/MoverTransform.h"
#include "../components/Balanza.h"
#include "../components/RotarTransform.h"
#include "../architecture/Time.h"
#include "../architecture/GameConstants.h"
#include "../components/SelfDestruct.h"
#include "../architecture/GeneralData.h"
#include "../sistemas/ComonObjectsFactory.h"
#include "../components/Depth.h"
#include <QATools/DataCollector.h>
#include "../components/ErrorNote.h"
#include "../entities/ClockAux.h"

ecs::MainScene::MainScene():Scene(),fails_(0),correct_(0), timerPaused_(false)
{
	timer_ = MINIGAME_TIME;
#ifdef DEV_TOOLS
	stampsUnloked_= true;
	timeToAdd_ = 5;
#endif // DEV_TOOLS
	mPaqBuild_ = new PaqueteBuilder(this);
}

ecs::MainScene::~MainScene()
{
	delete mPaqBuild_;
}


void ecs::MainScene::update()
{
	Scene::update();
	if (!timerPaused_)
	{
		if (timer_ > 0) {
			timer_ -= Time::getDeltaTime();
		}
		else
			gm().requestChangeScene(ecs::sc::MAIN_SCENE, ecs::sc::END_WORK_SCENE);
	}
}

void ecs::MainScene::render()
{
	Scene::render();
#ifdef DEV_TOOLS
	ImGui::NewFrame();
	makeDataWindow();
	makeControlsWindow();
	ImGui::Render();

	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
#endif // DEV_TOOLS

}

void ecs::MainScene::init()
{
	std::cout << "Hola Main" << std::endl;
	sdlutils().clearRenderer(build_sdlcolor(0xFFFFFFFF));
	timer_ = MINIGAME_TIME;
	// Fondo
	factory_->setLayer(layer::BACKGROUND);
	factory_->createImage(Vector2D(), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH),
		&sdlutils().images().at("fondoOficina"));

	//for (int i = 0; i < 7; i++) {
	//	createTubo((pq::Distrito)i);
	//}

	createManual();
	createMiniManual();
	createSpaceManual();

	createClock();

	createInks();

	//QUITAR ESTO PARA LA VERSION FINAL, ESTO ES PARA FACILITAR LA DEMO
	//createCinta();

	createGarbage();
	createPaquete(generalData().getPaqueteLevel());

	//creacion de las herramientas
	// En el caso de que los tubos no estén ordenados, habrá que ordenarlos
	int numTubos = generalData().getTubesAmount(); // coge el numero de tubos que están desbloqueados
	int j = 0;
	for (int i = 0;i < numTubos; i++) {
		createTubo((pq::Distrito)i, true);
		j++;
	}
	//Creación de paquetes bloqueados
	for (int z = j; z < 7 ; ++z) { //grande jose la los numeros magicos te la sabes
		if(j==6)
			createTubo((pq::Distrito)z, true);
		else
			createTubo((pq::Distrito)z , false);
	}

	sdlutils().musics().at("office").play();
	sdlutils().musics().at("office").setMusicVolume(50);
	sdlutils().musics().at("printer").play();
	sdlutils().musics().at("printer").setMusicVolume(50);

	//Luis: dejo esto comentado porque con la refactorizacion se va a poder hacer de forma mas elegante

	//Se ha quitado toda la mierda, pero modificad en que dia exacto quereis crear las herramientas
	updateToolsPerDay(generalData().getDia());

}

void ecs::MainScene::close() {
	ecs::Scene::close();
	generalData().updateMoney();

	sdlutils().musics().at("office").haltMusic();
	sdlutils().musics().at("printer").haltMusic();
}

void ecs::MainScene::createClock() {
	Entity* clock = addEntity(ecs::layer::BACKGROUND);
	clock->addComponent<ClockAux>(MINIGAME_TIME);
}

void ecs::MainScene::createInks() {

	createOneInk(SelloCalleA);
	createOneInk(SelloCalleB);
	createOneInk(SelloCalleC);

}

void ecs::MainScene::createOneInk(TipoHerramienta type) {
	Entity* ink = factory_->createImage(Vector2D(70 + 150 * type, 950), Vector2D(125, 73), &sdlutils().images().at("tinta"+std::to_string(type)));
	Trigger* inkATri = ink->addComponent<Trigger>();

	inkATri->addCallback([this, type](ecs::Entity* entRec) {

		if (entRec->getLayer() == ecs::layer::STAMP) {

			Herramientas* stampHerramienta = entRec->getComponent<Herramientas>();

			RenderImage* stampRender = entRec->getComponent<RenderImage>();

			if (!stampHerramienta->getMulticolorStamp()) { //Si el sello no es multicolor
				stampHerramienta->setFunctionality(type);

				stampRender->setTexture(&sdlutils().images().at("sellador" + std::to_string(type)));

			}

		}

	}, generalData().DropIn);

}

void ecs::MainScene::updateToolsPerDay(int dia)
{
	if(dia == 0)
		return;
	switch (dia)
	{
	case 1:

		//if(GeneralData::instance()->getSelloMulticolor()) createMultipleStamp();	  //Este es el sello multicolor. Si el jugador lo ha desbloqueado, este aparecerá en la oficina				

		createStamp(SelloCalleA);

		createInks();

		createBalanza();

		generalData().setPaqueteLevel(0);

		break;

	case 2:
		createStamp (SelloCalleA);

		createInks ();

		generalData().setPaqueteLevel(1);

		break;

	case 3:
		createStamp (SelloCalleA);

		createInks ();

		createCinta();

		generalData().setPaqueteLevel(2);

		break;
		//si estamos en un dia mayor que el indicado se desbloquean todas las mecánicas
	default:
		createStamp(SelloCalleA);

		createInks();

		createCinta();

		generalData().setPaqueteLevel(2);
		break;
	}	
}


void ecs::MainScene::createErrorMessage(Paquete* paqComp, bool basura, bool tuboIncorrecto) {
	Entity* NotaErronea = addEntity(ecs::layer::FOREGROUND);
	NotaErronea->addComponent<ErrorNote>(paqComp, basura, tuboIncorrecto);
	Texture* NotaTex = &sdlutils().images().at("notaError");
	Transform* NotaTR = NotaErronea->addComponent<Transform>(100, 1400, NotaTex->width() * 2, NotaTex->height() * 2);
	NotaTR->setScale(0.2f);
	NotaErronea->addComponent<Depth>();
	NotaErronea->addComponent<Gravity>();
	NotaErronea->addComponent<DragAndDrop>(true, "arrastrar");
	NotaErronea->addComponent<RenderImage>(NotaTex);
	NotaErronea->addComponent<MoverTransform>(NotaErronea->getComponent<Transform>()->getPos() - Vector2D(0, 500),
		1, Easing::EaseOutBack)->enable();
	//El texto de la nota
	factory_->setLayer(layer::FOREGROUND);
	Entity* texto = factory_->createLabel(Vector2D(25, 70), Vector2D(250, 100), NotaErronea->getComponent<ErrorNote>()->text_, 40);
	texto->getComponent<Transform>()->setParent(NotaErronea->getComponent<Transform>());
	factory_->setLayer(layer::DEFAULT);
}

void ecs::MainScene::createStamp(TipoHerramienta type)
{
	if (type > 2) return;
	constexpr float STAMPSIZE = 1;
	
	factory_->setLayer(layer::STAMP);
	auto stamp = factory_->createImage(Vector2D(300, 300),
		Vector2D(sdlutils().images().at("sellador" + std::to_string(type)).width() * STAMPSIZE, sdlutils().images().at("sellador" + std::to_string(type)).height() * STAMPSIZE),
		& sdlutils().images().at("sellador" + std::to_string(type)));

	stamp->addComponent<Gravity>();
	stamp->addComponent<Depth>();
	stamp->addComponent<DragAndDrop>("arrastrar");

	Herramientas* herrSelladorA = stamp->addComponent<Herramientas>();
	herrSelladorA->setFunctionality(type);

	factory_->setLayer(ecs::layer::DEFAULT);
}

void ecs::MainScene::createMultipleStamp()
{	
	constexpr float STAMPSIZE = 1;

	Entity* stamp = addEntity(ecs::layer::STAMP);
	Texture* StampTex = &sdlutils().images().at("selladorM");			
	Transform* tr_ = stamp->addComponent<Transform>(500, 300, StampTex->width(), StampTex->height());	
	stamp->addComponent<RenderImage>(StampTex);
	stamp->addComponent<Gravity>();
	stamp->addComponent<Depth>();
	stamp->addComponent<DragAndDrop>("arrastrar");

	Herramientas* herrSelladorA = stamp->addComponent<Herramientas>();
	herrSelladorA->setFunctionality(SelloMultiColor);

	factory_->setLayer(ecs::layer::DEFAULT);
}

void ecs::MainScene::createCinta() {

	factory_->setLayer(ecs::layer::TAPE);
	Entity* cinta = factory_->createImage(Vector2D(560, 500), Vector2D(100, 150), &sdlutils().images().at("cinta"));
	cinta->addComponent<Gravity>();
	cinta->addComponent<DragAndDrop>("arrastrar");
	cinta->addComponent<Depth>();
	factory_->setLayer(ecs::layer::DEFAULT);

}

void ecs::MainScene::createBalanza() {
	// Balanza
	factory_->setLayer(ecs::layer::BALANZA);
	Entity* balanza = factory_->createImage(Vector2D(50, 230), Vector2D(sdlutils().images().at("balanzaA").width(), sdlutils().images().at("balanzaA").height()), &sdlutils().images().at("balanzaA"));
	Transform* balanzaTr = balanza->getComponent<Transform>();
	balanzaTr->setScale(0.5);
	Balanza* balanzaComp = balanza->addComponent<Balanza>();

	// BalanzaB
	factory_->setLayer(ecs::layer::BALANZA);
	Entity* balanzaB = factory_->createImage(Vector2D(0, 0), Vector2D(sdlutils().images().at("balanzaB").width(), sdlutils().images().at("balanzaB").height()), &sdlutils().images().at("balanzaB"));
	Transform* balanzaBTr = balanzaB->getComponent<Transform>();
	balanzaBTr->setScale(0.5);

	// BalanzaBase
	factory_->setLayer(ecs::layer::BALANZABASE);
	Entity* baseBalanza = factory_->createImage(Vector2D(400, 400), Vector2D(sdlutils().images().at("baseBalanza").width(), sdlutils().images().at("baseBalanza").height()), &sdlutils().images().at("baseBalanza"));
	Transform* balanzaBaseTr = baseBalanza->getComponent<Transform>();
	balanzaBaseTr->setScale(0.5);
	baseBalanza->addComponent<Gravity>();
	//baseBalanza->addComponent<Depth>();

	// BalanzaFlecha
	factory_->setLayer(ecs::layer::BALANZA);
	Entity* balanzaFlecha = factory_->createImage(Vector2D(70, 20), Vector2D(sdlutils().images().at("balanzaFlecha").width(), sdlutils().images().at("balanzaFlecha").height()), &sdlutils().images().at("balanzaFlecha"));
	Transform* balanzaFlechaTr = balanzaFlecha->getComponent<Transform>();
	balanzaFlechaTr->setScale(0.5);
	RotarTransform* rotComp = balanzaFlecha->addComponent<RotarTransform>();

	// Seteamos padres
	balanzaTr->setParent(balanzaBaseTr);
	balanzaBTr->setParent(balanzaTr);
	balanzaFlechaTr->setParent(balanzaBaseTr);


	Trigger* balanzaTri = balanza->addComponent<Trigger>();

	balanzaTri->addCallback([this, rotComp, balanzaComp](ecs::Entity* entRect) {balanzaComp->initAnimations(entRect, rotComp); }, generalData().DropIn);
	balanzaTri->addCallback([this, rotComp, balanzaComp](ecs::Entity* entRect) {balanzaComp->finishAnimatios(rotComp); }, generalData().PickUp);

	factory_->setLayer(ecs::layer::DEFAULT);
}

void ecs::MainScene::createTubo(pq::Distrito dist,bool unlock) {
	constexpr float TUBE_WIDTH = 138;
	constexpr float TUBE_HEITH = 282;
	constexpr float TUBES_X_OFFSET = 200;
	constexpr float DISTANCE_BETWEEN_TUBES = 220;
	factory_->setLayer(ecs::layer::BACKGROUND);

	Entity* tuboEnt = factory_->createImage(
		Vector2D(TUBES_X_OFFSET + (DISTANCE_BETWEEN_TUBES * dist), -40),
		Vector2D(TUBE_WIDTH, TUBE_HEITH),
		&sdlutils().images().at("tubo" + std::to_string(dist + 1)));
	if (unlock) {

		Trigger* tuboTri = tuboEnt->addComponent<Trigger>();
		PackageChecker* tuboCheck = tuboEnt->addComponent<PackageChecker>(dist, this);
	}
	else {
		factory_->setLayer(layer::UI);
		auto tubeTr = tuboEnt->getComponent<Transform>();

		auto cross = factory_->createImage(Vector2D(0, 120),
			Vector2D(tubeTr->getWidth(), tubeTr->getWidth()),
			&sdlutils().images().at("cruz"));

		cross->getComponent<Transform>()->setParent(tubeTr);

	}
}


void ecs::MainScene::createManual()
{
	constexpr int MANUALNUMPAGES = 8;
	constexpr float MANUAL_WIDTH = 570;
	constexpr float MANUAL_HEITH = 359;

	Texture* buttonTexture = &sdlutils().images().at("cambioPag");
	//creado array de texturas par el libro
	std::vector<Texture*> bookTextures;
	bookTextures.reserve(MANUALNUMPAGES);
	for (int i = 1; i <= MANUALNUMPAGES; i++) {
		bookTextures.emplace_back(&sdlutils().images().at("book"+std::to_string(i)));
	}
	factory_->setLayer(ecs::layer::MANUAL);

	manualEnt_ = factory_->createMultiTextureImage(Vector2D(500, 500), Vector2D(MANUAL_WIDTH, MANUAL_HEITH),bookTextures);
	Transform* manualTransform = manualEnt_->getComponent<Transform>();
	RenderImage* manualRender = manualEnt_->getComponent<RenderImage>();
	manualRender->setVector(bookTextures);
	manualEnt_->addComponent<Gravity>();
	manualEnt_->addComponent<DragAndDrop>(false, "arrastrar");
	manualEnt_->addComponent<Depth>();


	Vector2D buttonSize(40, 40);
	factory_->setLayer(ecs::layer::FOREGROUND);
	auto next = [manualRender]() {manualRender->nextTexture();};
	auto right = factory_->createImageButton(Vector2D(490, 280), buttonSize, buttonTexture, next);
	right->getComponent<Transform>()->setParent(manualTransform);

	auto previous = [manualRender]() {manualRender->previousTexture();};
	auto left = factory_->createImageButton(Vector2D(40, 280), buttonSize, buttonTexture, previous);
	left->getComponent<Transform>()->setParent(manualTransform);
	left->getComponent<Transform>()->setFlip(SDL_FLIP_HORIZONTAL);

	factory_->setLayer(ecs::layer::DEFAULT);

}

void ecs::MainScene::createMiniManual() {

	constexpr float MANUAL_WIDTH = 70;
	constexpr float MANUAL_HEITH = 118;

	float minimanualX = 1200;
	float minimanualY = 500;

	factory_->setLayer(ecs::layer::MINIMANUAL);

	Texture* bookTextures = &sdlutils().images().at("miniManual");

	miniManualEnt_ = factory_->createImage(Vector2D(minimanualX, minimanualY), Vector2D(MANUAL_WIDTH, MANUAL_HEITH), bookTextures);

	Transform* manualTransform = miniManualEnt_->getComponent<Transform>();
	RenderImage* manualRender = miniManualEnt_->getComponent<RenderImage>();

	miniManualEnt_->addComponent<DragAndDrop>(false, true, "arrastrar");

	Trigger* mmTri = miniManualEnt_->getComponent<Trigger>();
	//Luis: TODO refactorizacion del codigo -> seguramente meter en un componente 

	mmTri->addCallback([this, mmTri, manualTransform, minimanualX, minimanualY](ecs::Entity* entRec) {

		if (miniManualEnt_->isActive()) {


			std::list<ecs::layer::layerId> entTouchingID = mmTri->getEntitiesTouching();

			if (entTouchingID.empty()) {

				Transform* manualTR = manualEnt_->getComponent<Transform>();

				Vector2D pos{ manualTransform->getPos().getX() - manualTR->getWidth() / 2, manualTransform->getPos().getY() - manualTR->getHeigth() / 2 };


				manualTransform->setPos(minimanualX, minimanualY);

				miniManualEnt_->setActive(false);

				manualEnt_->setActive(true);
				manualTR->setPos(pos);
				manualEnt_->getComponent<Depth>()->updateChildPos();
			}
			else {

				auto it = entTouchingID.begin();

				while (it != entTouchingID.end() && (*it) != ecs::layer::MANUALSPACE) {
					++it;
				}

				if (it == entTouchingID.end()) {


					manualTransform->setPos(minimanualX, minimanualY);
					miniManualEnt_->setActive(false);

					manualEnt_->setActive(true);


				}

			}
			
		}

	}, generalData().DropIn);


	factory_->setLayer(ecs::layer::DEFAULT);

	miniManualEnt_->setActive(false);

}

void ecs::MainScene::createSpaceManual() {

	constexpr float MANUAL_WIDTH = 70;
	constexpr float MANUAL_HEITH = 118;

	factory_->setLayer(ecs::layer::BACKGROUND);

	Texture* bookTextures = &sdlutils().images().at("atrilManual");
	
	auto baseManual = factory_->createImage(Vector2D(1200, 500), Vector2D(MANUAL_WIDTH, MANUAL_HEITH), bookTextures);
	
	Transform* manualTransform = baseManual->getComponent<Transform>();
	RenderImage* manualRender = baseManual->getComponent<RenderImage>();
	
	Trigger* mmTri = baseManual->addComponent<Trigger>();
	
	mmTri->addCallback([this, manualTransform](ecs::Entity* entRec) {


		if (entRec->getLayer() == ecs::layer::MANUAL && manualEnt_->isActive()) {

			Transform* manualTR = manualEnt_->getComponent<Transform>();

			manualTR->setPos(0, 0);

			manualEnt_->setActive(false);

			miniManualEnt_->setActive(true);

		}

	}, generalData().DropIn);
	

	factory_->setLayer(ecs::layer::DEFAULT);
	
}


void ecs::MainScene::createGarbage()
{
	/*TDOO Meter en un metdo */
	// papelera
	Entity* papelera = addEntity(ecs::layer::BIN);
	papelera->addComponent<Transform>(50, 650, 100, 150);
	papelera->addComponent<RenderImage>(&sdlutils().images().at("papelera"));
	Trigger* papTrig = papelera->addComponent<Trigger>();
	papelera->addComponent<PackageChecker>(Erroneo, this);
}
#ifdef DEV_TOOLS


void ecs::MainScene::makeDataWindow()
{
	ImGui::Begin("Paquetes Scene Data");
	//Reloj del timepo de la partida
	std::string time = "Current Game Time: " + std::to_string(timer_);
	ImGui::Text(time.c_str());
	//Contador de aciertos
	std::string data = "Aciertos: " + std::to_string(generalData().getCorrects());
	ImGui::Text(data.c_str());
	//contador de Fallos
	data = "Fallos: " + std::to_string(generalData().getFails());
	ImGui::Text(data.c_str());
	//Nivel de los paquetes
	data = "Pacage Level: " + std::to_string(generalData().getPaqueteLevel());
	ImGui::Text(data.c_str());
	//Dia acutual del juego
	data = "Current day: " + std::to_string(GeneralData::instance()->getCurrentDay());
	ImGui::Text(data.c_str());
	ImGui::End();
}

void ecs::MainScene::makeControlsWindow()
{
	static bool customPackage;
	ImGui::Begin("Controls");
	if (ImGui::CollapsingHeader("Paquetes"))
	{
		//panel para crear un paquete custom
		static int dist,calle,tipo,nivPeso,peso = 0;
		static bool correcto,fragil, carta = false;
		ImGui::Checkbox("Custom Package", &customPackage);
		if (customPackage) {
			//Los valores pueden ser de 0 a 7
			ImGui::InputInt("Distrito", &dist);
			//Los valores pueden ser de 0 a 3
			ImGui::InputInt("Calle",&calle);
			//Los valores son de 0 a 4
			ImGui::InputInt("Tipo",&tipo);
			ImGui::Checkbox("SelloCorrecto", &correcto);
			ImGui::InputInt("NivPeso",&nivPeso);
			ImGui::InputInt("Peso", &peso);
			ImGui::Checkbox("Fragil", &fragil);
			ImGui::Checkbox("Carta", &carta);
		}
		//ImGui::Checkbox("Next Pacage Correct", &nextPacageCorrect_);
		if (ImGui::Button("Create pacage")) {
			if (customPackage) {
				auto custom = mPaqBuild_->customPackage((pq::Distrito)dist,(pq::Calle)calle,"Sujeto de Pruebas", (pq::TipoPaquete)tipo, 
					correcto, (pq::NivelPeso)nivPeso, peso, fragil, carta);
				custom->getComponent<MoverTransform>()->enable();
			}
			else {
				createPaquete(generalData().getPaqueteLevel());
			}
		}
	}
	//Todavia no es funcinal ya que no hay forma actual de limitar las mecánicas
	if (ImGui::CollapsingHeader("Mecánicas"))
	{
		int lvl = generalData().getPaqueteLevel();
		ImGui::InputInt("Nivel del Paquete", &lvl);
		generalData().setPaqueteLevel(lvl);
	}
	if (ImGui::CollapsingHeader("Tiempo")) {
		if (ImGui::Button("Reset Timer")) {
			timer_ = MINIGAME_TIME;
		}

		ImGui::InputInt("Aditional Seconds", &timeToAdd_);
		if (ImGui::Button("Add Time")) {
			timer_ += timeToAdd_;
		}
	}

	//Todavia no es funcinal ya que no hay forma actual de limitar las mecánicas
	/*if (ImGui::CollapsingHeader("Días"))
	{
		int day = generalData().getDia();
		ImGui::InputInt("Día", &day);
		generalData().setDia(day);
	}*/
	ImGui::End();
}
#endif // DEV_TOOLS



void ecs::MainScene::createPaquete (int lv) {
	auto pac = mPaqBuild_->buildPackage(lv, this);
	pac->getComponent<MoverTransform>()->enable();
}