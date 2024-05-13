#include "IntroScene.h"
#include <utils/checkML.h>
#include <architecture/Entity.h>
#include <sdlutils/SDLUtils.h>
#include <components/Transform.h>
#include <components/Render.h>
#include <components/Clickeable.h>
#include <components/DragAndDrop.h>
#include <components/Trigger.h>
#include <string>
#include <components/MoverTransform.h>
#include <architecture/GameConstants.h>
#include <architecture/GeneralData.h>
#include <sistemas/ComonObjectsFactory.h>
#include <QATools/DataCollector.h>
#include <entities/ClockAux.h>
#include <components/SelfDestruct.h>

ecs::IntroScene::IntroScene() : introIteration(0), waitingCallback(false), mPaqBuild_(nullptr), tubo_(nullptr), bottle_(nullptr)
{
	mPaqBuild_ = new PaqueteBuilder(this);
	mDialogManager.init(this, "recursos/data/dialogos.json");
}

ecs::IntroScene::~IntroScene()
{
	delete mPaqBuild_;
}

void ecs::IntroScene::update()
{
	Scene::update();
	if (waitingCallback)
	{
		timer_ = sdlutils().virtualTimer().currTime();
		if (timer_ > timeToCall_)
		{
			waitingCallback = false;
			call_();
		}
	}
	mDialogManager.update();
}

void ecs::IntroScene::init()
{
	factory_->setLayer(layer::BACKGROUND);
	updateIteration(introIteration);
}

void ecs::IntroScene::updateIteration(int it)
{
    switch (it)
    {
        case 0: //pantalla en negro dialogo de introduccion
			factory_->setLayer(layer::BACKGROUND);
			factory_->createImage(Vector2D(), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH),
				&sdlutils().images().at("blackScreen"));
			factory_->setLayer(ecs::layer::DEFAULT);
			delayedCallback(0.5f, [this]
				{
					updateIntroDialogue();
				});
			break;
        case 1: //oficina, se explica el trabajo de paco
			factory_->setLayer(layer::BACKGROUND);
			factory_->createImage(Vector2D(), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH),
				&sdlutils().images().at("fondoTutorial"));
			factory_->setLayer(ecs::layer::DEFAULT);
			tubo_ = createGarbage();
			delayedCallback(0.5f, [this]
				{
					updateIntroDialogue();
				});
			break;
        case 2 ://un par de paquetes antes de la botella
			delayedCallback(0.5f, [this]
				{
					createIntroPackage();
				});
			break;
        case 3:
			delayedCallback(1.0f, [this]
				{
					createIntroPackage();
				});
		case 4:
			delayedCallback(1.0f, [this]
				{
					createIntroPackage();
				});
			break;
		case 5: //Paco esta harto de su trabajo y quiere escapar
			delayedCallback(1.0f, [this]
				{
					updateIntroDialogue();
				});
		    break;
        case 6: //aparece la botella
			delayedCallback(0.5f, [this]
				{
					bottle_ = createBottle();
				});
			break;
		case 7: //QUEEEEEEEEEEEEEE COMO QUE LA BOTELLA VIENE DE LA ATLANTIDA?!?!?1?!?!?11?!?!?
			delayedCallback(0.25f, [this]
				{
					bottle_->getComponent<RenderImage>()->nextTexture();
					updateIntroDialogue();
				});
			break;
        case 8: //pantalla en negro con carta e infodumping de lo que sea se haya inventado esta gente para que el prota acabe en la atlantida
			tubo_->setAlive(false);
			bottle_->setAlive(false);
			factory_->createImage(Vector2D(), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH),
				&sdlutils().images().at("blackScreen"));
			factory_->createImage(Vector2D(LOGICAL_RENDER_WIDTH/2 - 600/2, 50), Vector2D(600, 800), &sdlutils().images().at("cartaAtlantida"));
			delayedCallback(0.25f, [this]
				{
					updateIntroDialogue();
				});
		    break;
        case 9:
			gm().requestChangeScene(ecs::sc::INTRO_SCENE, ecs::sc::EXPLORE_SCENE);
		    break;
		default:
			break;
    }
}

void ecs::IntroScene::updateIntroDialogue()
{
	mDialogManager.setDialogueEntitiesActive(true);
	mDialogManager.setDialogues(DialogManager::Intro, std::to_string(introIteration));
	mDialogManager.setEndDialogueCallback([this]
		{
			nextIteration();
		});
}

void ecs::IntroScene::delayedCallback(float time, SimpleCallback call) {
	call_ = call;

	waitingCallback = true;
	timeToCall_ = sdlutils().virtualTimer().currTime() + (time * 1000);
}

ecs::Entity* ecs::IntroScene::createGarbage()
{
	factory_->setLayer(layer::OFFICEELEMENTS);
	Entity* papelera = addEntity(ecs::layer::BIN);
	papelera->addComponent<Transform>(0, 650, 204, 247);
	papelera->addComponent<RenderImage>(&sdlutils().images().at("papelera"));
	Trigger* papTrig = papelera->addComponent<Trigger>();
	papTrig->addCallback([this](ecs::Entity* e) {
		if (e->getComponent<Paquete>() != nullptr)
		    e->getComponent<MoverTransform>()->setEasing(Easing::EaseOutCubic);
		    e->getComponent<MoverTransform>()->setFinalPos(e->getComponent<Transform>()->getPos() + Vector2D(-600, 0));
			e->getComponent<MoverTransform>()->setMoveTime(1);
			e->getComponent<MoverTransform>()->enable();
		    e->addComponent<SelfDestruct>(1);
			nextIteration();
		}, gD().DropIn);
	factory_->setLayer(ecs::layer::DEFAULT);
	return papelera;
}

void ecs::IntroScene::createIntroPackage()
{
	Entity* package;
	DragAndDrop::enableDrag = true;
	factory_->setLayer(ecs::layer::PACKAGE);
	package = mPaqBuild_->buildPackage(0, this); //TODO: que los paquetes sean custom y tengan los nombres de Cleon, Rome y Fumono Studios
	package->getComponent<MoverTransform>()->enable();
	factory_->setLayer(ecs::layer::DEFAULT);
}

ecs::Entity* ecs::IntroScene::createBottle()
{
	factory_->setLayer(ecs::layer::PACKAGE);
	auto bottle = factory_->createMultiTextureImage(Vector2D(1600.0f, 550.0f), Vector2D(450, 300), { &sdlutils().images().at("botella1"), &sdlutils().images().at("botella2") }
);

	bottle->addComponent<Clickeable>("click");
	bottle->getComponent<Clickeable>()->addEvent([this]
		{
			nextIteration();
		});

	auto movComp = bottle->addComponent<MoverTransform>();
	movComp->setEasing(Easing::EaseOutCubic);
	movComp->setFinalPos(bottle->getComponent<Transform>()->getPos() + Vector2D(-900, 0));
	movComp->setMoveTime(1.7f);
	movComp->enable();
	factory_->setLayer(ecs::layer::DEFAULT);
	return bottle;
}

void ecs::IntroScene::nextIteration()
{
	introIteration++;
	updateIteration(introIteration);
}
