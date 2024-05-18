#include <sdlutils/InputHandler.h>
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "TutorialScene.h"
#include <sistemas/PaqueteBuilder.h>
#include <sistemas/ComonObjectsFactory.h>
#include <components/Transform.h>
#include <entities/ClockAux.h>
#include <components/DialogComponent.h>
#include <components/DelayedCallback.h>
#include <components/PackageChecker.h>
#include <components/ErrorNote.h>
#include <architecture/GameConstants.h>
#include <sistemas/SoundEmiter.h>
#include <architecture/Game.h>
#include <architecture/Time.h>

ecs::TutorialScene::TutorialScene() : MainScene(), balanzaUsed(false) {
	tutorialSys_ = new TutorialSystem(this);	
}

ecs::TutorialScene::~TutorialScene() {
	delete tutorialSys_;
}

void ecs::TutorialScene::update() {
	Scene::update();
	tutorialSys_->update();
	dialogMngr_.update();
#ifdef _DEBUG
	if (ih().isKeyDown(SDL_SCANCODE_O)) {
		gm().requestChangeScene(ecs::sc::TUTORIAL_SCENE, ecs::sc::MAIN_SCENE);
	}
#endif // _DEBUG
}

void ecs::TutorialScene::render() {
	MainScene::render();
}

void ecs::TutorialScene::init() {

	tutorialSys_->init();

	sdlutils().clearRenderer(build_sdlcolor(0xFFFFFFFF));

	//fondo
	factory_->setLayer(layer::BACKGROUND);
	factory_->createImage(Vector2D(), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH),
		&sdlutils().images().at("fondoOficina"));

	mPipeMngr_->init();

	int dia = gD().getDay();

	if (dia < 3 && dia >= 1) {
		createManual(8);
	}
	else if (dia < 5 && dia >= 3) {
		createManual(8);
	}
	else if (dia < 8 && dia >= 5) {
		createManual(9);
	}
	else if (dia < 15 && dia >= 8) {
		createManual(10);
	}

	createMiniManual();
	createSpaceManual();

	manualEnt_->setActive(false);

	miniManualEnt_->setActive(true);

	createGarbage();

	createInks();

	createStamp(SelloCalleA);

	TutorialScene::createTubes();

	if (gD().getDay() == 1) {

		tutorialSys_->activateEvent(TutorialSystem::Introduction);


	}
	else if (gD().getDay() == 3) {

		tutorialSys_->activateEvent(TutorialSystem::EntraTercerPaquete);

	}
	else if (gD().getDay() == 5) {
		tutorialSys_->activateEvent(TutorialSystem::EntraPaquetePeso);

	}
	else if (gD().getDay() == 8) {

		tutorialSys_->activateEvent(TutorialSystem::EntraPaqueteFragil);

	}
	
}

void ecs::TutorialScene::close() {
	ecs::Scene::close();

  SoundEmiter::instance()->close();
}

void ecs::TutorialScene::activateTubos() {

	for (int i = 0; i < gD().getTubesAmount(); i++)
	{
		Trigger* tuboTri = tubos[i]->addComponent<Trigger>();
		PackageChecker* tuboCheck = tubos[i]->addComponent<PackageChecker>(Distrito(i), this, mPipeMngr_);
	}
}


void ecs::TutorialScene::activateAllButOneTube(int tub)
{

	for (int i = 0; i < gD().getTubesAmount(); i++)
	{

		if (tub != i) {
			Trigger* tuboTri = tubos[i]->addComponent<Trigger>();
			PackageChecker* tuboCheck = tubos[i]->addComponent<PackageChecker>(Distrito(i), this, mPipeMngr_);
		}
		
	}

}

void ecs::TutorialScene::activateOneTube(int tube)
{

	if (tubos[tube]->getComponent<Trigger>() != nullptr) {

		Trigger* tuboTri = tubos[tube]->addComponent<Trigger>();

	}
	
	if (tubos[tube]->getComponent<PackageChecker>() != nullptr) {

		PackageChecker* tuboCheck = tubos[tube]->addComponent<PackageChecker>(Distrito(tube), this, mPipeMngr_);

	}
	

}

ecs::Entity* ecs::TutorialScene::createMiniManual()
{
	
	Entity* mmEnt_ = MainScene::createMiniManual();

	Trigger* mmTri_ = mmEnt_->getComponent<Trigger>();

	mmTri_->addCallback([this, mmTri_](ecs::Entity* entRec) {

			std::list<ecs::layer::layerId> entTouchingID = mmTri_->getEntitiesTouching();

			if (entTouchingID.empty()) {

				tutorialSys_->notifyAction(TutorialSystem::SacarManual);

			}
			else {

				auto it = entTouchingID.begin();

				while (it != entTouchingID.end() && (*it) != ecs::layer::MANUALSPACE) {
					++it;
				}

				if (it == entTouchingID.end()) {

					tutorialSys_->notifyAction(TutorialSystem::SacarManual);
				}
			}
	}, gD().DropIn);

	return mmEnt_;
}

//manual, roght, left
std::unordered_map<std::string, ecs::Entity*> ecs::TutorialScene::createManual(int NumPages)
{

	std::unordered_map<std::string, ecs::Entity*> mapSol = MainScene::createManual(NumPages);

	RenderImage* mRen = mapSol["manual"]->getComponent<RenderImage>();

	Clickeable* rClick = mapSol["right"]->getComponent<Clickeable>();

	Clickeable* lClick = mapSol["left"]->getComponent<Clickeable>();

	rClick->deleteEvents();

	auto next = [mRen, this]() {

		if(tutorialSys_->canPassPagesManual)
		mRen->nextTexture(); 
	
	};

	rClick->addEvent(next);

	rClick->addEvent([mRen, this]() {
		const Texture* tex = mRen->getCurrentTexture();
		if (tex == &sdlutils().images().at("book3"))
			tutorialSys_->notifyAction(TutorialSystem::PaginaCodigosPostales);
		else if (tex == &sdlutils().images().at("book4"))
			tutorialSys_->notifyAction(TutorialSystem::PaginaDistritoHestia);
		else if (tex == &sdlutils().images().at("book5"))
			tutorialSys_->notifyAction(TutorialSystem::PaginaDistritoDemeter);
		else if (tex == &sdlutils().images().at("book8"))
			tutorialSys_->notifyAction(TutorialSystem::PaginaSellos);
		else if(tex == &sdlutils().images().at("book9"))
			tutorialSys_->notifyAction(TutorialSystem::PaginaPesado);
		else if (tex == &sdlutils().images().at("book10"))
			tutorialSys_->notifyAction(TutorialSystem::PaginaFragilAccion);

	});

	lClick->deleteEvents();

	auto previous = [mRen, this]() {

		if (tutorialSys_->canPassPagesManual)
			mRen->previousTexture();

	};

	lClick->addEvent(previous);

	lClick->addEvent([mRen, this]() {
		const Texture* tex = mRen->getCurrentTexture();
		if (tex == &sdlutils().images().at("book3"))
			tutorialSys_->notifyAction(TutorialSystem::PaginaCodigosPostales);
		else if (tex == &sdlutils().images().at("book4"))
			tutorialSys_->notifyAction(TutorialSystem::PaginaDistritoHestia);
		else if (tex == &sdlutils().images().at("book5"))
			tutorialSys_->notifyAction(TutorialSystem::PaginaDistritoDemeter);
		else if (tex == &sdlutils().images().at("book8"))
			tutorialSys_->notifyAction(TutorialSystem::PaginaSellos);
		else if (tex == &sdlutils().images().at("book9"))
			tutorialSys_->notifyAction(TutorialSystem::PaginaPesado);
		else if (tex == &sdlutils().images().at("book10"))
			tutorialSys_->notifyAction(TutorialSystem::PaginaFragilAccion);
		
	});


	return mapSol;
}

std::unordered_map<std::string, ecs::Entity*> ecs::TutorialScene::createBalanza()
{

	std::unordered_map<std::string, ecs::Entity*> mapSol = MainScene::createBalanza();

	Trigger* balTri = mapSol["balanza"]->getComponent<Trigger>();

	balTri->addCallback([this](ecs::Entity* entRect) {balanzaUsed = true; }, gD().PickUp);

	return mapSol;

}

std::unordered_map<std::string, ecs::Entity*> ecs::TutorialScene::createTubes()
{

	std::unordered_map<std::string, ecs::Entity*> tubes = MainScene::createTubes();

	for (int i = 0; i < gD().getTubesAmount(); ++i) {

		std::string name = "tube" + i;

		Trigger* triTub = tubes[name]->getComponent<Trigger>();

		triTub->addCallback([this](ecs::Entity* entRect) { tutorialSys_->notifyAction(TutorialSystem::PaqueteEnviado); }, gD().DropIn);

	}

	return tubes;
}

void ecs::TutorialScene::deactivateTubos() {
	for (int i = 0; i < gD().getTubesAmount(); i++)
	{
		deactivateOneTube(i);
	}
}

void ecs::TutorialScene::deactivateAllButOneTube(int tub)
{

	for (int i = 0; i < gD().getTubesAmount(); i++)
	{
		if (i != tub) {
			deactivateOneTube(i);
		}
	}
}

void ecs::TutorialScene::deactivateOneTube(int tube)
{

	if (tubos[tube]->getComponent<Trigger>() != nullptr) {
		tubos[tube]->removeComponent<Trigger>();
	}
	
	if (tubos[tube]->getComponent<PackageChecker>() != nullptr) {
		tubos[tube]->removeComponent<PackageChecker>();
	}
	

}

ecs::Entity* ecs::TutorialScene::createGarbage()
{
	garbage_ = MainScene::createGarbage();

	Trigger* papTrig = garbage_->addComponent<Trigger>();
	papTrig->addCallback([this](ecs::Entity* e) {
		if (e->getComponent<Paquete>() != nullptr)
		tutorialSys_->notifyAction(TutorialSystem::Basura);
	}, gD().DropIn);

	return garbage_;
}

void ecs::TutorialScene::activateGarbage() {

	garbage_->addComponent<PackageChecker>(Erroneo, this, mPipeMngr_);

}

void ecs::TutorialScene::deactivateGarbage() {

	garbage_->removeComponent<PackageChecker>();

}

ecs::Entity* ecs::TutorialScene::createPackage(PackageTutorial pt) {

	ecs::Entity* paquete;
	factory_->setLayer(ecs::layer::PACKAGE);
	switch (pt)
	{
	case Primero:
		paquete = mPaqBuild_.customPackage(Hestia, C3, "Fernando Lubina", Alimento);
		break;
	case Segundo:
		paquete = mPaqBuild_.customPackage(Demeter, C2, "Miguel Torres", Medicinas);
		break;
	case Tercero:
		paquete = mPaqBuild_.customPackage(Artemisa, C1, "Francis Ngannou", Armamento, false);
		break;
	case FallarAposta:
		paquete = mPaqBuild_.customPackage(Demeter, C3, "Jhonny Huesos", Medicinas);
		break;
	case Fragil:
		paquete = mPaqBuild_.customPackage(Hestia, C3, "Travis Lubin", Alimento, true, pq::Ninguno, 0, true);
		break;
	case BalanzaTut:
		paquete = mPaqBuild_.customPackage(Hefesto, C2, "Rodiballo Garcia", Materiales, true, pq::Alto, 160);
		break;
	case Carta:
		paquete = mPaqBuild_.customPackage(Demeter, C1, "Percebesa Crujierez", Medicinas, true, pq::Ninguno, 0, false, true);
		break;
	default:
		paquete = mPaqBuild_.buildPackage(1, this);
		break;
	}


	paquete->getComponent<Trigger>()->addCallback([paquete, this](ecs::Entity* entRec) {

		auto& ihdlr = ih();

		SDL_Point point{ ihdlr.getMousePos().first, ihdlr.getMousePos().second };

		Herramientas* herrEnt = entRec->getComponent<Herramientas>();

		SDL_Rect stampRect = entRec->getComponent<Transform>()->getRect();

		if (herrEnt != nullptr && SDL_PointInRect(&point, &stampRect))
		{
			tutorialSys_->notifyAction(TutorialSystem::PaqueteEstampado);
			herrEnt->interact(paquete);
		}
		}, gD().DropIn);
	paquete->getComponent<Wrap>()->initComponent();
	paquete->getComponent<MoverTransform>()->enable();
	factory_->setLayer(ecs::layer::DEFAULT);

	return paquete;
}

void ecs::TutorialScene::packageSent() {
	tutorialSys_->notifyAction(TutorialSystem::PaqueteEnviado);
}
