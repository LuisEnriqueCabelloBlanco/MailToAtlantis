#include "IntroScene.h"
#include <sistemas/ComonObjectsFactory.h>
#include <QATools/DataCollector.h>
#include <components/SelfDestruct.h>
#include <sdlutils/InputHandler.h>
#include <architecture/GameConstants.h>

ecs::IntroScene::IntroScene() : mPaqBuild_(nullptr), fondo_(nullptr), tubo_(nullptr), bottle_(nullptr), carta_(nullptr), jefe_(nullptr), door_(nullptr)
{
	mPaqBuild_ = new PaqueteBuilder(this);

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
    #ifdef _DEBUG
	if (ih().keyDownEvent() && ih().isKeyDown(SDL_SCANCODE_O)) { //skipeamos el tutorial
		gm().requestChangeScene(ecs::sc::INTRO_SCENE, ecs::sc::EXPLORE_SCENE);
	}
    # endif
	mDialogManager.update();
}

void ecs::IntroScene::init()
{
	introIteration = 0;
	waitingCallback = false;
	mDialogManager.init(this, DIALOGS_PATH);
	factory_->setLayer(layer::BACKGROUND);
	updateIteration(introIteration);
}

void ecs::IntroScene::updateIteration(int it)
{
    switch (it)
    {
        case 0: //pantalla en negro dialogo de introduccion
			changeBackground("blackScreen");
			delayedCallback(0.5f, [this]
				{
					updateIntroDialogue();
				});
			break;
        case 1: //oficina, se explica el trabajo de paco
			changeBackground("fondoTutorial");
			tubo_ = createGarbage();
			delayedCallback(0.5f, [this]
				{
					updateIntroDialogue();
				});
			break;
        case 2 ://un par de paquetes antes de la botella
			delayedCallback(0.5f, [this]
				{
					createIntroPackage(Hefesto, "Carlos Leon", Joyas); //Cleon seria del distrito de Hefesto porque este es el dios de los trabajadores (Cleon no te enfades apruebame anda que te di regalices)
				});
			break;
        case 3:
			delayedCallback(1.0f, [this]
				{
					createIntroPackage(Hermes, "Alejandro Romero", Medicinas); //si rome fuera un paquete seria sin duda uno de medicinas
				});
		    break;
		case 4:
			delayedCallback(1.0f, [this]
				{
					createIntroPackage(Poseidon, "FuMonoStudios", Armamento); //QUEEEEEEEEE HAN DICHO EL NOMBRE DEL ESTUDIO OMG!!!1!1111!!11
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
			if(tubo_ != nullptr)
			    tubo_->setAlive(false);
			if(bottle_ != nullptr)
			    bottle_->setAlive(false);
			changeBackground("blackScreen");
			factory_->setLayer(ecs::layer::PACKAGE);
			carta_ = factory_->createImage(Vector2D(LOGICAL_RENDER_WIDTH/2 - 600/2, 50), Vector2D(600, 800), &sdlutils().images().at("cartaAtlantida"));
			factory_->setLayer(layer::DEFAULT);
			delayedCallback(0.25f, [this]
				{
					updateIntroDialogue();
				});
		    break;
        case 9: //Paco llega a la atlantida,ve al jefe quejandose de que el anterior trabajador acaba de dimitir y basicamente acepta el empleo
			if (carta_ != nullptr)
				carta_->setAlive(false);
			changeBackground("Hestia");
			factory_->setLayer(ecs::layer::PACKAGE);
			jefe_ = factory_->createImage(Vector2D(500.0f, 550.0f), Vector2D(450, 500), &sdlutils().images().at("Jefe"));
			factory_->setLayer(ecs::layer::DEFAULT);
			delayedCallback(0.25f, [this]
				{
					updateIntroDialogue();
				});
			break;
        case 10:
			changeBackground("HestiaHighlight");

			jefe_->setActive(false);
			door_ = createDoor();
			break;
        case 11:
			if (door_ != nullptr)
				door_->setAlive(false);
			changeBackground("fondoOficina");
			jefe_->getComponent<Transform>()->setPos(450.0f, 220.0f);
			jefe_->getComponent<Transform>()->setScale(0.75f);
			jefe_->setActive(true);
			updateIntroDialogue();
		    break;
        case 12: //a explorar rey
			gm().requestChangeScene(ecs::sc::INTRO_SCENE, ecs::sc::EXPLORE_SCENE);
			break;
		default:
			break;
    }
}

void ecs::IntroScene::updateIntroDialogue() //sacar el dialogo de la iteracion actual y avanza a la siguiente iteracion cuando se cierra el dialogo
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

void ecs::IntroScene::changeBackground(const std::string& background) //cambia el fondo por el que se le pasa
{
	factory_->setLayer(layer::BACKGROUND);
	fondo_ = factory_->createImage(Vector2D(), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH),
		&sdlutils().images().at(background));
	factory_->setLayer(ecs::layer::DEFAULT);
}

ecs::Entity* ecs::IntroScene::createGarbage() //crea el tubo de la basura
{
	factory_->setLayer(layer::BIN);
	Entity* papelera = factory_->createImage(Vector2D(0, 650), Vector2D(204, 247), &sdlutils().images().at("papelera"));
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

void ecs::IntroScene::createIntroPackage(Distrito dist, const std::string& remitente, TipoPaquete tp) //crea los paquetes que aparecen en la intro
{
	Entity* package;
	DragAndDrop::enableDrag = true;
	factory_->setLayer(ecs::layer::PACKAGE);
	package = mPaqBuild_->customPackage(dist, C1, remitente, tp, true, Ninguno, 0, false, false); 
	factory_->setLayer(ecs::layer::DEFAULT);
	package->getComponent<MoverTransform>()->enable();
}

ecs::Entity* ecs::IntroScene::createBottle() 
{
	factory_->setLayer(ecs::layer::PACKAGE);
	
	auto bottle = factory_->createMultiTextureButton(Vector2D(1600.0f, 550.0f), Vector2D(450, 300), { &sdlutils().images().at("botella1"), &sdlutils().images().at("botella2") }, [this]
		{
			bottle_->getComponent<Clickeable>()->toggleClick(false);
			nextIteration();
		}, "click");

	auto movComp = bottle->addComponent<MoverTransform>();
	movComp->setEasing(Easing::EaseOutCubic);
	movComp->setFinalPos(bottle->getComponent<Transform>()->getPos() + Vector2D(-900, 0));
	movComp->setMoveTime(1.7f);
	movComp->enable();
	factory_->setLayer(ecs::layer::DEFAULT);
	return bottle;
}

ecs::Entity* ecs::IntroScene::createDoor()
{
	Entity* e;
	e = addEntity();
	e->addComponent<Transform>(650, 400, 100, 300);
	e->addComponent<Clickeable>("click");
	e->getComponent<Clickeable>()->addEvent([this]
		{
			door_->setAlive(false);
			nextIteration();
		});
	return e;
}

void ecs::IntroScene::nextIteration() //avanzamos la iteracion y actualizamos el estado de la intro
{
	introIteration++;
	updateIteration(introIteration);
}
