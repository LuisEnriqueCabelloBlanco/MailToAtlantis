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

ecs::TutorialScene::TutorialScene() : MainScene(), balanzaUsed(false) {


	tutorialSys_ = new TutorialSystem(this);

	//mPaqBuild_ = new PaqueteBuilder(this);
	
}

ecs::TutorialScene::~TutorialScene() {
	delete tutorialSys_;
	//delete mPaqBuild_;
}

void ecs::TutorialScene::update() {
	MainScene::update();
	tutorialSys_->update();
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

	createManual(10);
	createMiniManual();
	createSpaceManual();

	manualEnt_->setActive(false);

	miniManualEnt_->setActive(true);

	createGarbage();

	createInks();

	createStamp(SelloCalleA);

	createTubes();

	if (generalData().getDay() == 1) {

		tutorialSys_->activateEvent(TutorialSystem::Introduction);


	}
	else if (generalData().getDay() == 3) {

		tutorialSys_->activateEvent(TutorialSystem::EntraTercerPaquete);

	}
	else if (generalData().getDay() == 5) {
		tutorialSys_->activateEvent(TutorialSystem::EntraPaquetePeso);

	}
	else if (generalData().getDay() == 8) {

		tutorialSys_->activateEvent(TutorialSystem::EntraPaqueteFragil);

	}
	
}

void ecs::TutorialScene::close() {
	ecs::Scene::close();
  SoundEmiter::instance()->close();
}

void ecs::TutorialScene::activateTubos() {

	for (int i = 0; i < generalData().getTubesAmount(); i++)
	{
		Trigger* tuboTri = tubos[i]->addComponent<Trigger>();
		PackageChecker* tuboCheck = tubos[i]->addComponent<PackageChecker>(Distrito(i), this, mPipeMngr_);
	}
}


void ecs::TutorialScene::activateAllButOneTube(int tub)
{

	for (int i = 0; i < generalData().getTubesAmount(); i++)
	{

		if (tub != i) {
			Trigger* tuboTri = tubos[i]->addComponent<Trigger>();
			PackageChecker* tuboCheck = tubos[i]->addComponent<PackageChecker>(Distrito(i), this, mPipeMngr_);
		}
		
	}

}

void ecs::TutorialScene::activateOneTube(int tube)
{

	Trigger* tuboTri = tubos[tube]->addComponent<Trigger>();
	PackageChecker* tuboCheck = tubos[tube]->addComponent<PackageChecker>(Distrito(tube), this, mPipeMngr_);

}

ecs::Entity* ecs::TutorialScene::createMiniManual()
{
	
	Entity* mmEnt_ = MainScene::createMiniManual();

	Trigger* mmTri_ = mmEnt_->getComponent<Trigger>();

	mmTri_->addCallback([this, mmTri_](ecs::Entity* entRec) {

			std::list<ecs::layer::layerId> entTouchingID = mmTri_->getEntitiesTouching();

			if (entTouchingID.empty()) {

				tutorialSys_->registerAction(TutorialSystem::SacarManual);

			}
			else {

				auto it = entTouchingID.begin();

				while (it != entTouchingID.end() && (*it) != ecs::layer::MANUALSPACE) {
					++it;
				}

				if (it == entTouchingID.end()) {

					tutorialSys_->registerAction(TutorialSystem::SacarManual);
				}
			}
	}, generalData().DropIn);

	return mmEnt_;
}

//manual, roght, left
std::unordered_map<std::string, ecs::Entity*> ecs::TutorialScene::createManual(int NumPages)
{

	std::unordered_map<std::string, ecs::Entity*> mapSol = MainScene::createManual(NumPages);

	RenderImage* mRen = mapSol["manual"]->getComponent<RenderImage>();

	Clickeable* rClick = mapSol["right"]->getComponent<Clickeable>();

	Clickeable* lClick = mapSol["left"]->getComponent<Clickeable>();

	rClick->addEvent([mRen, this]() {
		if (tutorialSys_->canPassPagesManual)
		{
			const Texture* tex = mRen->getCurrentTexture();
			if (tex == &sdlutils().images().at("book3"))
				tutorialSys_->registerAction(TutorialSystem::PaginaCodigosPostales);
			else if (tex == &sdlutils().images().at("book4"))
				tutorialSys_->registerAction(TutorialSystem::PaginaDistritoHestia);
			else if (tex == &sdlutils().images().at("book5"))
				tutorialSys_->registerAction(TutorialSystem::PaginaDistritoDemeter);
			else if (tex == &sdlutils().images().at("book8"))
				tutorialSys_->registerAction(TutorialSystem::PaginaSellos);
		}
	});

	lClick->addEvent([mRen, this]() {
		if (tutorialSys_->canPassPagesManual)
		{
			const Texture* tex = mRen->getCurrentTexture();
			if (tex == &sdlutils().images().at("book3"))
				tutorialSys_->registerAction(TutorialSystem::PaginaCodigosPostales);
			else if (tex == &sdlutils().images().at("book4"))
				tutorialSys_->registerAction(TutorialSystem::PaginaDistritoHestia);
			else if (tex == &sdlutils().images().at("book5"))
				tutorialSys_->registerAction(TutorialSystem::PaginaDistritoDemeter);
			else if (tex == &sdlutils().images().at("book8"))
				tutorialSys_->registerAction(TutorialSystem::PaginaSellos);
		}
	});


	return mapSol;
}

std::unordered_map<std::string, ecs::Entity*> ecs::TutorialScene::createBalanza()
{

	std::unordered_map<std::string, ecs::Entity*> mapSol = MainScene::createBalanza();

	Trigger* balTri = mapSol["balanza"]->getComponent<Trigger>();

	balTri->addCallback([this](ecs::Entity* entRect) {balanzaUsed = true; }, generalData().PickUp);

	return mapSol;

}

std::unordered_map<std::string, ecs::Entity*> ecs::TutorialScene::createTubes()
{

	std::unordered_map<std::string, ecs::Entity*> tubes = MainScene::createTubes();

	for (int i = 0; i < generalData().getTubesAmount(); ++i) {

		std::string name = "tube" + i;

		Trigger* triTub = tubes[name]->getComponent<Trigger>();

		triTub->addCallback([this](ecs::Entity* entRect) { tutorialSys_->registerAction(TutorialSystem::PaqueteEnviado); }, generalData().DropIn);

	}

	return tubes;
}

void ecs::TutorialScene::deactivateTubos() {
	for (int i = 0; i < generalData().getTubesAmount(); i++)
	{
		tubos[i]->removeComponent<Trigger>();
		tubos[i]->removeComponent<PackageChecker>();
	}
}

void ecs::TutorialScene::deactivateAllButOneTube(int tub)
{

	for (int i = 0; i < generalData().getTubesAmount(); i++)
	{
		if (i != tub) {
			tubos[i]->removeComponent<Trigger>();
			tubos[i]->removeComponent<PackageChecker>();
		}
		
	}

}

void ecs::TutorialScene::deactivateOneTube(int tube)
{

	tubos[tube]->removeComponent<Trigger>();
	tubos[tube]->removeComponent<PackageChecker>();

}

ecs::Entity* ecs::TutorialScene::createGarbage()
{
	garbage_ = MainScene::createGarbage();

	Trigger* papTrig = garbage_->addComponent<Trigger>();
	papTrig->addCallback([this](ecs::Entity* e) {
		if (e->getComponent<Paquete>() != nullptr)
		tutorialSys_->registerAction(TutorialSystem::Basura);
	}, generalData().DropIn);

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
	if (pt == Primero)
		paquete = mPaqBuild_->customPackage(Hestia, C3, "Fernando Lubina", Alimento);
	else if (pt == Segundo)
		paquete = mPaqBuild_->customPackage(Demeter, C2, "Miguel Torres", Medicinas);
	else if (pt == Tercero)
		paquete = mPaqBuild_->customPackage(Artemisa, C1, "Francis Ngannou", Armamento, false);
	else if (pt == FallarAposta)
		paquete = mPaqBuild_->customPackage(Demeter, C3, "Jhonny Huesos", Medicinas);
	else if (pt == Fragil)
		paquete = mPaqBuild_->customPackage(Hestia, C3, "Travis Lubin", Alimento,true, pq::Ninguno,0,true);
	else if (pt == BalanzaTut)
		paquete = mPaqBuild_->customPackage(Hefesto, C2, "Rodiballo Garcia", Materiales, true, pq::Medio, 90);
	else
		paquete = mPaqBuild_->buildPackage(1, this);


	paquete->getComponent<Trigger>()->addCallback([paquete, this](ecs::Entity* entRec) {

		auto& ihdlr = ih();

		SDL_Point point{ ihdlr.getMousePos().first, ihdlr.getMousePos().second };

		Herramientas* herrEnt = entRec->getComponent<Herramientas>();

		SDL_Rect stampRect = entRec->getComponent<Transform>()->getRect();

		if (herrEnt != nullptr && SDL_PointInRect(&point, &stampRect))
		{
			tutorialSys_->registerAction(TutorialSystem::PaqueteEstampado);
			herrEnt->interact(paquete);
		}
		}, generalData().DropIn);
	paquete->getComponent<Wrap>()->initComponent();
	paquete->getComponent<MoverTransform>()->enable();
	factory_->setLayer(ecs::layer::DEFAULT);

	return paquete;
}

void ecs::TutorialScene::createErrorMessage(Paquete* paqComp, bool basura, bool tuboIncorrecto) {
	Entity* NotaErronea = addEntity(ecs::layer::FOREGROUND);
	NotaErronea->addComponent<ErrorNote>(paqComp, basura, tuboIncorrecto);
	Texture* NotaTex = &sdlutils().images().at("notaError");
	Transform* NotaTR = NotaErronea->addComponent<Transform>(100, 1400, NotaTex->width() * 2, NotaTex->height() * 2);
	NotaTR->setScale(0.2f);
	NotaErronea->addComponent<Depth>();
	NotaErronea->addComponent<Gravity>();
	NotaErronea->addComponent<DragAndDrop>(true,"arrastrar");
	NotaErronea->addComponent<RenderImage>(NotaTex);
	NotaErronea->addComponent<MoverTransform>(NotaErronea->getComponent<Transform>()->getPos() - Vector2D(0, 500),
		1, Easing::EaseOutBack)->enable();
	//El texto de la nota
	Entity* texto_ = addEntity(ecs::layer::FOREGROUND);
	Font* textFont = new Font("recursos/fonts/ARIAL.ttf", 40);
	Texture* textureText_ = new Texture(sdlutils().renderer(), NotaErronea->getComponent<ErrorNote>()->text_, *textFont, build_sdlcolor(0x000000ff), 500);
	Transform* distritoTr = texto_->addComponent<Transform>(25, 70, 250, 100);
	RenderImage* distritoRender = texto_->addComponent<RenderImage>();
	distritoRender->setTexture(textureText_);
	distritoTr->setParent(NotaErronea->getComponent<Transform>());
}

void ecs::TutorialScene::packageSent() {
	tutorialSys_->registerAction(TutorialSystem::PaqueteEnviado);
}
