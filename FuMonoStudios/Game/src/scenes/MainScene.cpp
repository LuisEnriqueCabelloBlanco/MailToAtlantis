#ifdef DEV_TOOLS
#include <imgui.h>
#include <imgui_impl_sdlrenderer2.h>
#else
#include <utils/checkML.h>
#endif // DEV_TOOLS
#include "MainScene.h"
#include <architecture/Entity.h>
#include <iostream>
#include <fstream>
#include <sdlutils/SDLUtils.h>
#include <components/Transform.h>
#include <components/Render.h>
#include <components/Clickeable.h>
#include <components/DragAndDrop.h>
#include <components/Trigger.h>
#include <string>
#include <list>
#include <unordered_map>
#include <sdlutils/Texture.h>
#include <components/PackageChecker.h>
#include <components/Gravity.h>
#include <components/MoverTransform.h>
#include <components/Balanza.h>
#include <components/RotarTransform.h>
#include <architecture/Time.h>
#include <architecture/GameConstants.h>
#include <architecture/GeneralData.h>
#include <sistemas/ComonObjectsFactory.h>
#include <components/Depth.h>
#include <QATools/DataCollector.h>
#include <components/ErrorNote.h>
#include <entities/ClockAux.h>
#include <sistemas/PipeManager.h>
#include <sistemas/SoundEmiter.h>
#include <components/HoverLayerComponent.h>
#include <components/RenderWithLight.h>
#include <components/NPCExclamation.h>
#include <sistemas/NPCeventSystem.h>

ecs::MainScene::MainScene():Scene(),fails_(0),correct_(0), timerPaused_(false)
{
	timer_ = MINIGAME_TIME;
#ifdef DEV_TOOLS
	stampsUnloked_= true;
	timeToAdd_ = 5;
#endif // DEV_TOOLS
	mPaqBuild_ = new PaqueteBuilder(this);
	mPipeMngr_ = new PipeManager();
	specialFactory_ = new SpecialObjectsFactory(this);
}

ecs::MainScene::~MainScene()
{
	delete mPaqBuild_;
	delete mPipeMngr_;
	delete specialFactory_;
}


void ecs::MainScene::update()
{
	Scene::update();
	if (gm().gamePaused()) {
		timerPaused_ = true;
	}
	else {
		timerPaused_ = false;
	}
	if (!timerPaused_)
	{
		if (timer_ > 0) {
			timer_ -= Time::getDeltaTime();
		}
		else
		{
			gm().requestChangeScene(ecs::sc::MAIN_SCENE, ecs::sc::END_WORK_SCENE);
		}
	}
	dialogMngr_.update();

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

	generalData().npcEventSys->shuffleNPCqueue();
	generalData().npcEventSys->debugPaquetesInQueue();
#ifdef _DEBUG
	std::cout << "Hola Main" << std::endl;
#endif // _DEBUG
	sdlutils().clearRenderer(build_sdlcolor(0xFFFFFFFF));
	timer_ = MINIGAME_TIME;
	timerPaused_ = true;

	// Fondo
	factory_->setLayer(layer::BACKGROUND);
	factory_->createImage(Vector2D(), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH),
		&sdlutils().images().at("fondoOficina"));

	//for (int i = 0; i < 7; i++) {
	//	createTubo((pq::Distrito)i);
	//}	

	//La bola de cristal se tiene que crear antes que el primer paquete
	if (GeneralData::instance ()->getUpgradeValue (ecs::upg::BOLA_UPGRADE)) createBolaCristal();	  //Este es la bola de cristal. Si el jugador la ha desbloqueado, esta aparecerá en la oficina				
	//}
	mPipeMngr_->init();

	
	createMiniManual();
	createSpaceManual();

	//createClock(); empieza a girar desde que se entra a la escena y queremos que lo haga cuando entres al trabajo

	createGarbage();	

	int dia = generalData().getDay();
	if (dia % 4 == 2) //hay un evento de trabajo del jefe cada 4 dias empezando por el dia 2, esto habria que hacerlo con constantes mejor en vez de numeros magicos 
	{
		createCharacter({ 500, 250 }, "Jefe",0.35f);
	}
	else
		startWork();

	//creacion de las herramientas
	// En el caso de que los tubos no estén ordenados, habrá que ordenarlos
	int numTubos = generalData().getTubesAmount(); // coge el numero de tubos que están desbloqueados
	int j = 0;
	for (int i = 0;i < numTubos; i++) {
		tubos.push_back(createTubo((pq::Distrito)i, true));
		j++;
	}
	//Creación de paquetes bloqueados
	for (int z = j; z < 7 ; ++z) { //grande jose la los numeros magicos te la sabes
		if(j==6)
			tubos.push_back(createTubo((pq::Distrito)z, true));
		else
			tubos.push_back(createTubo((pq::Distrito)z , false));
	}

	/*sdlutils().musics().at("office").play();
	sdlutils().musics().at("office").setMusicVolume(50);
	sdlutils().musics().at("printer").play();
	sdlutils().musics().at("printer").setMusicVolume(50);*/
	SoundEmiter::instance()->playMusic("office");
	SoundEmiter::instance()->playMusic("printer");

	//Se ha quitado toda la mierda, pero modificad en que dia exacto quereis crear las herramientas
	updateToolsPerDay(generalData().getDay());

	specialFactory_->setupDayObjects();
}

void ecs::MainScene::close() {
	ecs::Scene::close();
	generalData().npcEventSys->minigameOver();
	generalData().updateMoney();

	sdlutils().musics().at("office").haltMusic();
	sdlutils().musics().at("printer").haltMusic();
}

ecs::Entity* ecs::MainScene::createClock() {
	Entity* clock = addEntity(ecs::layer::BACKGROUND);
	clock->addComponent<ClockAux>(MINIGAME_TIME);
	return clock;
}
ecs::Entity* ecs::MainScene::createBolaCristal() {
	int tamano = 3;
	std::vector<Texture*> ballTextures;
	ballTextures.reserve(tamano);
	for (int i = 1; i <= tamano; i++) {
		ballTextures.emplace_back(&sdlutils().images().at("bola" + std::to_string(i)));
	}
	Entity* bola = factory_->createMultiTextureImage(Vector2D(700, 500), Vector2D(150, 200), ballTextures);
	bolaCrist_ = bola->addComponent<CristalBall>(bola->getComponent<RenderImage>());
	
	return bola;
}

void ecs::MainScene::createInks() {

	createOneInk(SelloCalleA);
	createOneInk(SelloCalleB);
	createOneInk(SelloCalleC);

}

ecs::Entity* ecs::MainScene::createOneInk(TipoHerramienta type) {
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

	return ink;

}

void ecs::MainScene::updateToolsPerDay(int dia)
{
	GeneralData::instance ()->setUpgradeValue (ecs::upg::BALANZA_UPGRADE, true);
	dia = 5;
	if(dia == 0)
		return;	
	
	if (dia >= 1) {
		if (GeneralData::instance()->getUpgradeValue(ecs::upg::SELLO_UPGRADE)) createMultipleStamp();	  //Este es el sello multicolor. Si el jugador lo ha desbloqueado, este aparecerá en la oficina								
		else createStamp(SelloCalleA);

		createInks();

	}

	if (dia >= 5) {
		if (GeneralData::instance ()->getUpgradeValue (ecs::upg::BALANZA_UPGRADE)) createBalanzaDigital ();
		else createBalanza();
	}

	if (dia >= 8) {				
		createCinta();
	}


	if (dia < 3 && dia >= 1) {
		generalData().setPaqueteLevel(0);
		createManual(8);
	}
	else if (dia < 5 && dia >= 3) {
		generalData().setPaqueteLevel(1);
		createManual(8);
	}
	else if (dia < 8 && dia >= 5) {
		generalData().setPaqueteLevel(2);
		createManual(9);
	}
	else if (dia < 15 && dia >= 8) {
		generalData().setPaqueteLevel(3);
		createManual(10);
	}

}
void ecs::MainScene::createExclamationPoint() {
	Entity* xd = addEntity(ecs::layer::FOREGROUND);	
	auto ld = xd->addComponent<NPCExclamation>();
	ld->innit(100,100);
	
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

ecs::Entity* ecs::MainScene::createStamp(TipoHerramienta type)
{
	if (type > 2) return nullptr;
	constexpr float STAMPSIZE = 1;
	
	factory_->setLayer(layer::STAMP);
	auto stamp = factory_->createImage(Vector2D(350, 700),
		Vector2D(sdlutils().images().at("sellador" + std::to_string(type)).width() * STAMPSIZE, sdlutils().images().at("sellador" + std::to_string(type)).height() * STAMPSIZE),
		& sdlutils().images().at("sellador" + std::to_string(type)));

	stamp->addComponent<Gravity>();
	stamp->addComponent<Depth>();
	stamp->addComponent<DragAndDrop>("arrastrar");

	Herramientas* herrSelladorA = stamp->addComponent<Herramientas>();
	herrSelladorA->setFunctionality(type);

	factory_->setLayer(ecs::layer::DEFAULT);

	return stamp;
}

void ecs::MainScene::createMultipleStamp()
{	
	constexpr float STAMPSIZE = 1;

	Entity* stamp = addEntity(ecs::layer::STAMP);
	Texture* StampTex = &sdlutils().images().at("selladorM");			
	Transform* tr_ = stamp->addComponent<Transform>(300, 300, StampTex->width(), StampTex->height());
	stamp->addComponent<RenderImage>(StampTex);
	stamp->addComponent<Gravity>();
	stamp->addComponent<Depth>();
	stamp->addComponent<DragAndDrop>("arrastrar");

	Herramientas* herrSelladorA = stamp->addComponent<Herramientas>();
	herrSelladorA->setFunctionality(SelloMultiColor);

	factory_->setLayer(ecs::layer::DEFAULT);
}

ecs::Entity* ecs::MainScene::createCinta() {

	factory_->setLayer(ecs::layer::TAPE);
	Entity* cinta;
	if(GeneralData::instance ()->getUpgradeValue (ecs::upg::ENVOLVER_UPGRADE)) cinta = factory_->createImage (Vector2D (560, 500), Vector2D (100, 150), &sdlutils ().images ().at ("cintaRapida"));
	else cinta = factory_->createImage (Vector2D (560, 500), Vector2D (100, 150), &sdlutils ().images ().at ("cinta"));
	cinta->addComponent<Gravity>();
	cinta->addComponent<DragAndDrop>("arrastrar");
	cinta->addComponent<Depth>();
	factory_->setLayer(ecs::layer::DEFAULT);

	return cinta;
}

std::unordered_map<std::string, ecs::Entity*> ecs::MainScene::createBalanza() {

	std::unordered_map<std::string, ecs::Entity*> mapSol;

	float scale = 0.3;

	// Balanza
	factory_->setLayer(ecs::layer::BALANZA);
	Entity* balanza = factory_->createImage(Vector2D(30, 110), Vector2D(sdlutils().images().at("balanzaA").width(), sdlutils().images().at("balanzaA").height()), &sdlutils().images().at("balanzaA"));
	Transform* balanzaTr = balanza->getComponent<Transform>();
	balanza->addComponent<MoverTransform>();
	balanzaTr->setScale(scale);
	Balanza* balanzaComp = balanza->addComponent<Balanza>();

	// BalanzaB
	factory_->setLayer(ecs::layer::BALANZA);
	Entity* balanzaB = factory_->createImage(Vector2D(0, 0), Vector2D(sdlutils().images().at("balanzaB").width(), sdlutils().images().at("balanzaB").height()), &sdlutils().images().at("balanzaB"));
	Transform* balanzaBTr = balanzaB->getComponent<Transform>();
	balanzaBTr->setScale(scale);

	// BalanzaBase
	factory_->setLayer(ecs::layer::BALANZABASE);
	Entity* baseBalanza = factory_->createImage(Vector2D(1100, 300), Vector2D(sdlutils().images().at("baseBalanza").width(), sdlutils().images().at("baseBalanza").height()), &sdlutils().images().at("baseBalanza"));
	Transform* balanzaBaseTr = baseBalanza->getComponent<Transform>();
	balanzaBaseTr->setScale(scale);

	// BalanzaFlecha
	factory_->setLayer(ecs::layer::BALANZA);
	Entity* balanzaFlecha = factory_->createImage(Vector2D(45, 20), Vector2D(sdlutils().images().at("balanzaFlecha2").width(), sdlutils().images().at("balanzaFlecha2").height()), &sdlutils().images().at("balanzaFlecha2"));
	Transform* balanzaFlechaTr = balanzaFlecha->getComponent<Transform>();
	balanzaFlechaTr->setScale(scale);
	RotarTransform* rotComp = balanzaFlecha->addComponent<RotarTransform>();

	// Seteamos padres
	balanzaTr->setParent(balanzaBaseTr);
	balanzaBTr->setParent(balanzaTr);
	balanzaFlechaTr->setParent(balanzaBaseTr);


	Trigger* balanzaTri = balanza->addComponent<Trigger>();

	balanzaTri->addCallback([this, rotComp, balanzaComp, balanzaB](ecs::Entity* entRect) {balanzaComp->initAnimations(entRect, balanzaB, rotComp); }, generalData().DropIn);
	balanzaTri->addCallback([this, rotComp, balanzaComp](ecs::Entity* entRect) {balanzaComp->finishAnimatios(entRect, rotComp); }, generalData().PickUp);

	factory_->setLayer(ecs::layer::DEFAULT);

	mapSol.insert({ "balanza", balanza });
	mapSol.insert({ "balanzaB", balanzaB });
	mapSol.insert({ "baseBalanza", baseBalanza });
	mapSol.insert({ "balanzaFlecha", balanzaFlecha });

	return mapSol;
}

void ecs::MainScene::createBalanzaDigital() {
	// Balanza
	factory_->setLayer(ecs::layer::BALANZA);
	Entity* balanza = factory_->createImage(Vector2D(0, -44), Vector2D(sdlutils().images().at("balanzaDigA").width(), sdlutils().images().at("balanzaDigA").height()), &sdlutils().images().at("balanzaDigA"));
	Transform* balanzaTr = balanza->getComponent<Transform>();
	balanza->addComponent<MoverTransform>();
	balanzaTr->setScale(0.35);
	Balanza* balanzaComp = balanza->addComponent<Balanza>();

	// BalanzaBase
	factory_->setLayer(ecs::layer::BALANZABASE);
	Entity* baseBalanza = factory_->createImage(Vector2D(1050, 435), Vector2D(sdlutils().images().at("balanzaDigB").width(), sdlutils().images().at("balanzaDigB").height()), &sdlutils().images().at("balanzaDigB"));
	Transform* balanzaBaseTr = baseBalanza->getComponent<Transform>();
	balanzaBaseTr->setScale(0.35);
	baseBalanza->addComponent<Gravity>();
	//baseBalanza->addComponent<Depth>();

	////Añadir los numeros del peso
	std::string msg = "0";
	factory_->setLayer(ecs::layer::NUMBERS);
	factory_->createLabel(Vector2D(1270, 593), msg, 50);

	// Seteamos padres
	balanzaTr->setParent(balanzaBaseTr);

	
	Trigger* balanzaTri = balanza->addComponent<Trigger>();

	balanzaTri->addCallback([this, balanzaComp, balanza](ecs::Entity* entRect){
		balanzaComp->initAnimationsDigital(entRect, balanza); 
	std::string msg;
		int peso = balanzaComp->getPaquetePeso();
		if (peso >= 0) {
			msg = std::to_string(peso);
			removeEntitiesByLayer(ecs::layer::NUMBERS);
			factory_->setLayer(ecs::layer::NUMBERS);
			factory_->createLabel(Vector2D(1245, 593), msg, 50);
		}
		else {
			msg = "0";
			removeEntitiesByLayer(ecs::layer::NUMBERS);
			factory_->setLayer(ecs::layer::NUMBERS);
			factory_->createLabel(Vector2D(1270, 593), msg, 50);
		}
		
		}, generalData().DropIn);
	
	balanzaTri->addCallback([this, balanzaComp](ecs::Entity* entRect) {
		balanzaComp->finishAnimatiosDigital(entRect); 
		std::string msg2 = "0";
		removeEntitiesByLayer(ecs::layer::NUMBERS);
		factory_->setLayer(ecs::layer::NUMBERS);
		factory_->createLabel(Vector2D(1270, 593), msg2, 50);
		}, generalData().PickUp);

	factory_->setLayer(ecs::layer::DEFAULT);

}

ecs::Entity* ecs::MainScene::createTubo(pq::Distrito dist,bool unlock) {
	constexpr float TUBE_WIDTH = 138;
	constexpr float TUBE_HEITH = 282;
	constexpr float TUBES_X_OFFSET = 50;
	constexpr float DISTANCE_BETWEEN_TUBES = 280;
	factory_->setLayer(ecs::layer::DEFAULT);

	auto tubeTexture = &sdlutils().images().at("tubo" + std::to_string(dist + 1));
	Entity* tuboEnt = factory_->createImage(
		Vector2D(TUBES_X_OFFSET + (DISTANCE_BETWEEN_TUBES * dist), -40),
		Vector2D(TUBE_WIDTH, TUBE_HEITH),
		tubeTexture);
	if (unlock) {
		tubeTexture->modColor(255, 255, 255);
		auto layerHover = tuboEnt->addComponent<HoverLayerComponent>(ecs::layer::PACKAGE);
		auto hilight = tuboEnt->addComponent<RenderWithLight>();
		layerHover->addInCall([hilight]() {hilight->lightOn(); });
		layerHover->addOutCall([hilight]() {hilight->lightOff(); });

		Trigger* tuboTri = tuboEnt->addComponent<Trigger>();
		PackageChecker* tuboCheck = tuboEnt->addComponent<PackageChecker>(dist, this, mPipeMngr_);
	}
	else {
		tubeTexture->modColor(100, 100, 100);
	}

	return tuboEnt;
}


std::unordered_map<std::string, ecs::Entity*> ecs::MainScene::createManual(int NumPages)
{

	std::unordered_map<std::string, Entity*> mapSol;

	constexpr float MANUAL_WIDTH = 570;
	constexpr float MANUAL_HEITH = 359;

	Texture* buttonTexture = &sdlutils().images().at("cambioPag");
	//creado array de texturas par el libro
	std::vector<Texture*> bookTextures;

	bookTextures.reserve(NumPages);
	for (int i = 1; i <= NumPages; i++) {
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
	factory_->addHoverColorMod(right);

	auto previous = [manualRender]() {manualRender->previousTexture();};
	auto left = factory_->createImageButton(Vector2D(40, 280), buttonSize, buttonTexture, previous);
	left->getComponent<Transform>()->setParent(manualTransform);
	left->getComponent<Transform>()->setFlip(SDL_FLIP_HORIZONTAL);
	factory_->addHoverColorMod(left);

	factory_->setLayer(ecs::layer::DEFAULT);

	/*
	//Creacion de botones de indices

	if (true) { //PLACE HOLDER HASTA LOS BOOLS DE JULIAN

		Vector2D buttonIndexSize(20, 40);
		factory_->setLayer(ecs::layer::FOREGROUND);

		std::vector<int> indexTextures = { 2, 3, 6, 7, 8 };

		auto pagCodigos = [manualRender]() { manualRender->setTextureIndx(2); };
		auto indexCodigos = factory_->createImageButton(Vector2D(490, 280), buttonIndexSize, buttonTexture, pagCodigos);
		indexCodigos->getComponent<Transform>()->setParent(manualTransform);
		factory_->addHoverColorMod(indexCodigos);


	}
	*/
	
	mapSol.insert({ "manual", manualEnt_ });
	mapSol.insert({ "right", right });
	mapSol.insert({ "left", left });

	return mapSol;

}

ecs::Entity* ecs::MainScene::createMiniManual() {

	constexpr float MANUAL_WIDTH = 70;
	constexpr float MANUAL_HEITH = 118;

	float minimanualX = 1450;
	float minimanualY = 525;

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

	return miniManualEnt_;
}

ecs::Entity* ecs::MainScene::createSpaceManual() {

	constexpr float MANUAL_WIDTH = 70;
	constexpr float MANUAL_HEITH = 118;

	factory_->setLayer(ecs::layer::BACKGROUND);

	Texture* bookTextures = &sdlutils().images().at("atrilManual");
	
	auto baseManual = factory_->createImage(Vector2D(1450, 525), Vector2D(MANUAL_WIDTH, MANUAL_HEITH), bookTextures);
	
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
	

	return baseManual;
}


ecs::Entity* ecs::MainScene::createGarbage()
{
	/*TDOO Meter en un metdo */
	// papelera
	Entity* papelera = addEntity(ecs::layer::BIN);
	papelera->addComponent<Transform>(0, 650, 204, 247);
	papelera->addComponent<RenderImage>(&sdlutils().images().at("papelera"));
	Trigger* papTrig = papelera->addComponent<Trigger>();
	papelera->addComponent<PackageChecker>(Erroneo, this, mPipeMngr_);

	return papelera;
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
	data = "Current day: " + std::to_string(GeneralData::instance()->getDay());
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

	if (GeneralData::instance ()->getUpgradeValue (ecs::upg::BOLA_UPGRADE) && bolaCrist_!= nullptr) {
		int rnd = sdlutils().rand().nextInt(0, 4);		
		if(rnd !=1) bolaCrist_->check(pac->getComponent<Paquete>(), true);
		else bolaCrist_->check(pac->getComponent<Paquete>(), false);
	}
}



ecs::Entity* ecs::MainScene::createCharacter(Vector2D pos, const std::string& character, float scale) {

	std::string jsonPath = "recursos/data/eventosjefe.json";
	dialogMngr_.init(this, jsonPath);

	mWorkRes.init();
	Texture* characterTexture = &sdlutils().images().at(character);
	Vector2D size{ characterTexture->width() * scale, characterTexture->height() * scale };

	ecs::Entity* characterEnt = factory_->createImageButton(pos, size, characterTexture, [this]() {
		newWorkEvent();
		});

	dialogMngr_.setEndDialogueCallback([characterEnt, this]{
		characterEnt->setAlive(false); //bye bye jefe
		startWork();
	});

	return characterEnt;
}

void ecs::MainScene::startWork()
{
	timerPaused_ = false;
	createPaquete(generalData().getPaqueteLevel());
	createClock();
}

void ecs::MainScene::newWorkEvent()
{
	WorkEvent eventoJefe = mWorkRes.getRandomEvent();
	dialogMngr_.setDialogueEntitiesActive(true);
	dialogMngr_.setDialogues(eventoJefe.dialogue);
	mPipeMngr_->activateEvent(eventoJefe);
}
