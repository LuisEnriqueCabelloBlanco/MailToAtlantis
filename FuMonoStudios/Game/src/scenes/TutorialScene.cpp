#include "TutorialScene.h"
#include "../sistemas/PaqueteBuilder.h"
#include "../sistemas/ComonObjectsFactory.h"
#include "../components/Transform.h"
#include "../entities/ClockAux.h"
#include "../components/DialogComponent.h"
#include "../components/DelayedCallback.h"

ecs::TutorialScene::TutorialScene() : Scene() {
	mPaqBuild_ = new PaqueteBuilder(this);
}

ecs::TutorialScene::~TutorialScene() {

}

void ecs::TutorialScene::update() {
	Scene::update();
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

	createInks();

	createCharacter();

	createDialogueBox();

	sistemaFuturo = addEntity();

	sistemaFuturo->addComponent<DelayedCallback>(1, [this]() {
		activateDialogue();
		dialogMngr_.setDialogues(DialogManager::BryantMyers);
		textDialogue->addComponent<DialogComponent>(&dialogMngr_, this);
		});
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
	manualEnt_->addComponent<DragAndDrop>(false);
	manualEnt_->addComponent<Depth>();


	Vector2D buttonSize(40, 40);
	factory_->setLayer(ecs::layer::FOREGROUND);
	auto next = [manualRender]() {manualRender->nextTexture(); };
	auto right = factory_->createImageButton(Vector2D(490, 280), buttonSize, buttonTexture, next);
	right->getComponent<Transform>()->setParent(manualTransform);

	auto previous = [manualRender]() {manualRender->previousTexture(); };
	auto left = factory_->createImageButton(Vector2D(75, 280), buttonSize, buttonTexture, previous);
	left->getComponent<Transform>()->setParent(manualTransform);

	factory_->setLayer(ecs::layer::DEFAULT);
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

	miniManualEnt_->addComponent<DragAndDrop>(false, true);

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
		});


	factory_->setLayer(ecs::layer::DEFAULT);

	miniManualEnt_->setActive(false);
}

void ecs::TutorialScene::createSpaceManual() {
	constexpr float MANUAL_WIDTH = 70;
	constexpr float MANUAL_HEITH = 118;

	factory_->setLayer(ecs::layer::MANUALSPACE);

	Texture* bookTextures = &sdlutils().images().at("cartel");

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
		});

	factory_->setLayer(ecs::layer::DEFAULT);
}

void ecs::TutorialScene::createClock() {
	Entity* clock = addEntity(ecs::layer::BACKGROUND);
	clock->addComponent<ClockAux>(MINIGAME_TIME);
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
		});
}

void ecs::TutorialScene::createCharacter() {

}

void ecs::TutorialScene::createDialogueBox() {

	// creamos la entidad caja dialogo
	boxBackground = addEntity(ecs::layer::UI);
	auto bgTr = boxBackground->addComponent<Transform>(100, LOGICAL_RENDER_HEITH - 250, LOGICAL_RENDER_WIDTH - 100, 200);
	boxBackground->addComponent<RenderImage>(&sdlutils().images().at("cuadroDialogo"));

	// entidad del texto
	textDialogue = addEntity(ecs::layer::UI);
	auto textTr = textDialogue->addComponent<Transform>(100, 40, 80, 100);
	textTr->setParent(bgTr);
	textDialogue->addComponent<RenderImage>();

	boxBackground->setActive(false);
}

void ecs::TutorialScene::closeConversation() {
	textDialogue->getComponent<RenderImage>()->setTexture(nullptr);
	textDialogue->removeComponent<DialogComponent>();
	boxBackground->getComponent<RenderImage>()->setTexture(nullptr);
}

void ecs::TutorialScene::activateDialogue() {
	boxBackground->setActive(true);
}