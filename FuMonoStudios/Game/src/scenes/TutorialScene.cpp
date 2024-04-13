#include "TutorialScene.h"
#include "../sistemas/PaqueteBuilder.h"
#include "../sistemas/ComonObjectsFactory.h"
#include "../components/Transform.h"
#include "../entities/ClockAux.h"
#include "../components/DialogComponent.h"
#include "../components/DelayedCallback.h"
#include "../sdlutils/InputHandler.h"
#include "../components/PackageChecker.h"
#include "../components/ErrorNote.h"

ecs::TutorialScene::TutorialScene() : Scene() {

	tutorialSys_ = new TutorialSystem(this);
	mPaqBuild_ = new PaqueteBuilder(this);
}

ecs::TutorialScene::~TutorialScene() {

}

void ecs::TutorialScene::update() {
	Scene::update();
	tutorialSys_->update();
}

void ecs::TutorialScene::render() {
	Scene::render();
}

void ecs::TutorialScene::init() {
	sdlutils().clearRenderer(build_sdlcolor(0xFFFFFFFF));

	//fondo
	factory_->setLayer(layer::BACKGROUND);
	factory_->createImage(Vector2D(), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH),
		&sdlutils().images().at("fondoOficina"));

	createManual();
	createMiniManual();
	createSpaceManual();

	createClock();

	createGarbage();

	createInks();

	createStamp(SelloCalleA);

	//tubos
	for (int z = 0; z < 7; ++z) { //grande jose la los numeros magicos te la sabes
		// bien jose buenos numerazos magicos ahi estamos chaval a funcionar
		// jose
		// bien jose
		// padreas
		tubos.push_back(createTubo((pq::Distrito)z, true));
	}

	tutorialSys_->activateEvent(TutorialSystem::Introduction);
}

void ecs::TutorialScene::close() {
	ecs::Scene::close();
}

void ecs::TutorialScene::createManual() {
	constexpr int MANUALNUMPAGES = 8;
	constexpr float MANUAL_WIDTH = 570;
	constexpr float MANUAL_HEITH = 359;

	Texture* buttonTexture = &sdlutils().images().at("cambioPag");
	//creado array de texturas par el libro
	std::vector<Texture*> bookTextures;
	bookTextures.reserve(MANUALNUMPAGES);
	for (int i = 1; i <= MANUALNUMPAGES; i++) {
		bookTextures.emplace_back(&sdlutils().images().at("book" + std::to_string(i)));
	}
	factory_->setLayer(ecs::layer::MANUAL);

	manualEnt_ = factory_->createMultiTextureImage(Vector2D(500, 500), Vector2D(MANUAL_WIDTH, MANUAL_HEITH), bookTextures);
	Transform* manualTransform = manualEnt_->getComponent<Transform>();
	RenderImage* manualRender = manualEnt_->getComponent<RenderImage>();
	manualRender->setVector(bookTextures);
	manualEnt_->addComponent<Gravity>();
	manualEnt_->addComponent<DragAndDropTutorial>(false, tutorialSys_, "arrastrar");
	manualEnt_->addComponent<Depth>();


	Vector2D buttonSize(40, 40);
	factory_->setLayer(ecs::layer::FOREGROUND);
	auto next = [manualRender, this]() {
		if (tutorialSys_->canPassPagesManual)
		{
			manualRender->nextTexture();
			const Texture* tex = manualRender->getTexture();
			if (tex == &sdlutils().images().at("book3"))
				tutorialSys_->registerAction(TutorialSystem::PaginaCodigosPostales);
			else if (tex == &sdlutils().images().at("book4"))
				tutorialSys_->registerAction(TutorialSystem::PaginaDistritoHestia);
			else if (tex == &sdlutils().images().at("book5"))
				tutorialSys_->registerAction(TutorialSystem::PaginaDistritoDemeter);
			else if (tex == &sdlutils().images().at("book8"))
				tutorialSys_->registerAction(TutorialSystem::PaginaSellos);
		}
		};
	auto right = factory_->createImageButton(Vector2D(490, 280), buttonSize, buttonTexture, next);
	right->getComponent<Transform>()->setParent(manualTransform);

	auto previous = [manualRender, this]() {
		if (tutorialSys_->canPassPagesManual)
		{
			manualRender->previousTexture();
			const Texture* tex = manualRender->getTexture();
			if (tex == &sdlutils().images().at("book3"))
				tutorialSys_->registerAction(TutorialSystem::PaginaCodigosPostales);
			else if (tex == &sdlutils().images().at("book4"))
				tutorialSys_->registerAction(TutorialSystem::PaginaDistritoHestia);
			else if (tex == &sdlutils().images().at("book5"))
				tutorialSys_->registerAction(TutorialSystem::PaginaDistritoDemeter);
			else if (tex == &sdlutils().images().at("book8"))
				tutorialSys_->registerAction(TutorialSystem::PaginaSellos);
		}
		};
	auto left = factory_->createImageButton(Vector2D(40, 280), buttonSize, buttonTexture, previous);
	left->getComponent<Transform>()->setParent(manualTransform);
	left->getComponent<Transform>()->setFlip(SDL_FLIP_HORIZONTAL);

	factory_->setLayer(ecs::layer::DEFAULT);

	manualEnt_->setActive(false);
}

void ecs::TutorialScene::createMiniManual() {
	constexpr float MANUAL_WIDTH = 70;
	constexpr float MANUAL_HEITH = 118;

	float minimanualX = 1200;
	float minimanualY = 500;

	factory_->setLayer(ecs::layer::MINIMANUAL);

	Texture* bookTextures = &sdlutils().images().at("miniManual");

	miniManualEnt_ = factory_->createImage(Vector2D(minimanualX, minimanualY), Vector2D(MANUAL_WIDTH, MANUAL_HEITH), bookTextures);

	Transform* manualTransform = miniManualEnt_->getComponent<Transform>();
	RenderImage* manualRender = miniManualEnt_->getComponent<RenderImage>();

	miniManualEnt_->addComponent<DragAndDropTutorial>(false, true, tutorialSys_,"arrastrar");

	Trigger* mmTri = miniManualEnt_->getComponent<Trigger>();


	mmTri->addCallback([this, mmTri, manualTransform, minimanualX, minimanualY](ecs::Entity* entRec) {

		if (miniManualEnt_->isActive()) {


			std::list<ecs::layer::layerId> entTouchingID = mmTri->getEntitiesTouching();

			if (entTouchingID.empty()) {

				Transform* manualTR = manualEnt_->getComponent<Transform>();

				Vector2D pos{ manualTransform->getPos().getX() - manualTR->getWidth() / 2, manualTransform->getPos().getY() - manualTR->getHeigth() / 2 };


				manualTransform->setPos(minimanualX, minimanualY);

				miniManualEnt_->setActive(false);

				manualEnt_->setActive(true);
				tutorialSys_->registerAction(TutorialSystem::SacarManual);

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

					tutorialSys_->registerAction(TutorialSystem::SacarManual);
				}
			}
		}
		}, generalData().DropIn);


	factory_->setLayer(ecs::layer::DEFAULT);
}

void ecs::TutorialScene::createSpaceManual() {
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

ecs::Entity* ecs::TutorialScene::createTubo(pq::Distrito dist, bool unlock) {
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

		// EMPIEZA DESACTIVADO EL TUBO
	}
	else {
		factory_->setLayer(layer::UI);
		auto tubeTr = tuboEnt->getComponent<Transform>();

		auto cross = factory_->createImage(Vector2D(0, 120),
			Vector2D(tubeTr->getWidth(), tubeTr->getWidth()),
			&sdlutils().images().at("cruz"));

		cross->getComponent<Transform>()->setParent(tubeTr);

	}
	return tuboEnt;
}

void ecs::TutorialScene::activateTubos() {

	for (int i = 0; i < 7; i++)
	{
		Trigger* tuboTri = tubos[i]->addComponent<Trigger>();
		PackageChecker* tuboCheck = tubos[i]->addComponent<PackageChecker>(Distrito(i), this);
	}
}

void ecs::TutorialScene::deactivateTubos() {
	for (int i = 0; i < 7; i++)
	{
		tubos[i]->removeComponent<Trigger>();
		tubos[i]->removeComponent<PackageChecker>();
	}
}

void ecs::TutorialScene::createGarbage()
{
	garbage_ = addEntity(ecs::layer::BIN);
	garbage_->addComponent<Transform>(50, 650, 100, 150);
	garbage_->addComponent<RenderImage>(&sdlutils().images().at("papelera"));
}

void ecs::TutorialScene::activateGarbage() {
	Trigger* papTrig = garbage_->addComponent<Trigger>();
	papTrig->addCallback([this](ecs::Entity* e) {
		if(e->getComponent<Paquete>() != nullptr)
		tutorialSys_->registerAction(TutorialSystem::Basura);
		}, generalData().DropIn);
	garbage_->addComponent<PackageChecker>(Erroneo, this);
}

void ecs::TutorialScene::deactivateGarbage() {
	garbage_->removeComponent<PackageChecker>();
	garbage_->removeComponent<Trigger>();
}

void ecs::TutorialScene::createClock() {
	Entity* clock = addEntity(ecs::layer::BACKGROUND);
	clock->addComponent<ClockAux>(1000);
}

void ecs::TutorialScene::createInks() {
	createOneInk(SelloCalleA);
	createOneInk(SelloCalleB);
	createOneInk(SelloCalleC);
}

void ecs::TutorialScene::createOneInk(TipoHerramienta type) {
	Entity* ink = factory_->createImage(Vector2D(70 + 150 * type, 950), Vector2D(125, 73), &sdlutils().images().at("tinta" + std::to_string(type)));
	Trigger* inkATri = ink->addComponent<Trigger>();

	inkATri->addCallback([this, type](ecs::Entity* entRec) {

		if (entRec->getLayer() == ecs::layer::STAMP) {

			Herramientas* stampHerramienta = entRec->getComponent<Herramientas>();

			RenderImage* stampRender = entRec->getComponent<RenderImage>();

			stampHerramienta->setFunctionality(type);

			stampRender->setTexture(&sdlutils().images().at("sellador" + std::to_string(type)));

		}
		}, generalData().DropIn);
}

void ecs::TutorialScene::createStamp(TipoHerramienta type)
{
	if (type > 2) return;
	constexpr float STAMPSIZE = 1;

	factory_->setLayer(layer::STAMP);

	auto stamp = factory_->createImage(Vector2D(230, 800),
		Vector2D(sdlutils().images().at("sellador" + std::to_string(type)).width() * STAMPSIZE, sdlutils().images().at("sellador" + std::to_string(type)).height() * STAMPSIZE),
		&sdlutils().images().at("sellador" + std::to_string(type)));

	stamp->addComponent<Gravity>();
	stamp->addComponent<Depth>();
	stamp->addComponent<DragAndDropTutorial>(true, tutorialSys_,"arrastrar");

	Herramientas* herrSelladorA = stamp->addComponent<Herramientas>();
	herrSelladorA->setFunctionality(type);

	factory_->setLayer(ecs::layer::DEFAULT);
}

void ecs::TutorialScene::closeConversation() {
	tutorialSys_->closeConversation();
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
	else
		paquete = mPaqBuild_->buildPackage(1, this);

	paquete->removeComponent<DragAndDrop>();
	paquete->removeComponent<Trigger>();
	paquete->addComponent<DragAndDropTutorial>(true, tutorialSys_,"arrastrar");
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

void ecs::TutorialScene::createFragilTool() {
	factory_->setLayer(ecs::layer::TAPE);
	Entity* cinta = factory_->createImage(Vector2D(250, 0), Vector2D(100, 150), &sdlutils().images().at("cinta"));
	cinta->addComponent<Gravity>();
	cinta->addComponent<DragAndDropTutorial>(true, tutorialSys_,"arrastrar");
	cinta->addComponent<Depth>();
	factory_->setLayer(ecs::layer::DEFAULT);
}

void ecs::TutorialScene::packageSent() {
	tutorialSys_->registerAction(TutorialSystem::PaqueteEnviado);
}
