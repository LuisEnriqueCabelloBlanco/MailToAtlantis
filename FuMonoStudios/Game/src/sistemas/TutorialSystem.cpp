#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "TutorialSystem.h"
#include "../components/Render.h"
#include "../scenes/TutorialScene.h"
#include "../components/DialogManager.h"
#include "../components/DialogComponent.h"
#include "../components/MoverTransform.h"
#include "../components/Wrap.h"

TutorialSystem::TutorialSystem(ecs::TutorialScene* scene) {
	scene_ = scene;
	
}

TutorialSystem::~TutorialSystem() {

}

void TutorialSystem::update() {
	if (waitingCallback)
	{
		timer_ = sdlutils().virtualTimer().currTime();
		if (timer_ > timeToCall_)
		{
			waitingCallback = false;
			call_();
		}
	}

	if (waitingEmbalaje) {
		if (waitingWrapComp->isWrapped())
		{
			waitingEmbalaje = false;
			registerAction(Action::Embalado);
		}
	}

	if (waitingPesado) {

		if (scene_->balanzaUsed) {
			waitingPesado = false;
			registerAction(Action::Pesado);
		}

	}


	dialogMngr_.update();
}

void TutorialSystem::activateEvent(TutorialEvent event) {

	currentEvent = event;
	tutorialIteration = (int)event;
	switch (event)
	{
		#pragma region Manual
		case TutorialEvent::Introduction:
			canPassPagesManual = false;
			DragAndDrop::enableDrag = false;
			activateDialogue(false);
			break;
		case TutorialEvent::SacaElManual1:
			DragAndDrop::enableDrag = false;
			activateDialogue(false);
			arrow_->setActive(true);
			
			// animacion de la flecha
			arrow_->getComponent<Transform>()->setPos(1350, 425);
			arrow_->getComponent<Transform>()->setRotation(130);
			arrow_->addComponent<MoverTransform>(Vector2D(800, 700), 1, Easing::EaseOutBack);
			arrow_->getComponent<MoverTransform>()->enable();

			delayedCallback(1.2, [this]() {
				arrow_->getComponent<Transform>()->setPos(1350, 425);
				arrow_->addComponent<MoverTransform>(Vector2D(800, 700), 1, Easing::EaseOutBack, [this]() {
					arrow_->getComponent<Transform>()->setPos(1350, 425);
					});
				arrow_->getComponent<MoverTransform>()->enable();
				});
			break;
		case TutorialEvent::SacaElManual2:
			DragAndDrop::enableDrag = false;
			activateDialogue(false);
			break;

#pragma endregion

		#pragma region Primer Paquete
		case TutorialEvent::PaqueteEnsenarRemitente:
			DragAndDrop::enableDrag = false;
			scene_->createPackage(ecs::TutorialScene::Primero);
			delayedCallback(1, [this]() {
				activateDialogue(false);
				delayedCallback(0.2, [this]() {
					arrow_->setActive(true);
					arrow_->getComponent<Transform>()->setPos(1340, 720);
					});
				});
			break;
		case TutorialEvent::PaqueteEnsenarCodigoPostal:
			activateDialogue(false);
			arrow_->getComponent<Transform>()->setPos(1340, 680);
			break;
		case TutorialEvent::PaqueteBuscarPaginaCodigosPostales:
			canPassPagesManual = true;
			activateDialogue(false);
			break;
		case TutorialEvent::BuscarPaginaHestia:
			arrow_->setActive(false);
			arrow_->getComponent<Transform>()->setRotation(130);
			canPassPagesManual = false;
			activateDialogue(false);
			break;
		case TutorialEvent::EnsenarSellos:
			canPassPagesManual = false;
			activateDialogue(true);
			arrow_->setActive(true);
			arrow_->getComponent<Transform>()->setPos(350, 850);
			arrow_->addComponent<MoverTransform>(Vector2D(100, 850), 1, Easing::EaseOutBack);
			arrow_->getComponent<MoverTransform>()->enable();
			delayedCallback(1.2, [this]() {
				arrow_->addComponent<MoverTransform>(Vector2D(350, 850), 1, Easing::EaseOutBack);
				arrow_->getComponent<MoverTransform>()->enable();
				});
			break;
		case TutorialEvent::EnsenarTubos:
			canDrag = false;
			canPassPagesManual = true;
			activateDialogue(false);
			break;
#pragma endregion

		#pragma region Segundo Paquete
		case TutorialEvent::EntraSegundoPaquete:
			DragAndDrop::enableDrag = false;
			scene_->createPackage(ecs::TutorialScene::Segundo);
			delayedCallback(1, [this]() {
				scene_->deactivateTubos();
				activateDialogue(false);
				});
			break;
		case TutorialEvent::SegundoBuscarPaginaDistritos:
			activateDialogue(false);
			break;
		case TutorialEvent::SellarSegundoPaquete:
			canPassPagesManual = false;
			activateDialogue(false);
			break;
		case TutorialEvent::EnviarSegundoPaquete:
			activateDialogue(false);
			break;
#pragma endregion

		#pragma region Tercer Paquete
		case TutorialEvent::EntraTercerPaquete:
			DragAndDrop::enableDrag = true; // CAMBIAR A TRUE PARA EMPEZAR DESDE MAS ADELANTE
			canPassPagesManual = false;
			scene_->deactivateTubos();
			scene_->createPackage(ecs::TutorialScene::Tercero);
			delayedCallback(0.5, [this]() {
				activateDialogue(false);
				arrow_->setActive(true);
				arrow_->getComponent<Transform>()->setRotation(130);
				arrow_->getComponent<Transform>()->setPos(1355, 620);
				});
			break;
		case TutorialEvent::EnPaginaInfoSellos:
			arrow_->setActive(false);
			canPassPagesManual = false;
			activateDialogue(false);
			break;
#pragma endregion

		#pragma region Paquete Fallar Aposta
		case TutorialEvent::EntraCuartoPaquete:
			DragAndDrop::enableDrag = false;
			activateDialogue(false);
			delayedCallback(1.5, [this] {
				scene_->createPackage(ecs::TutorialScene::FallarAposta);
				});
			break;
		case TutorialEvent::ExplicacionFalloAposta:
			activateDialogue(true);
			break;
		#pragma endregion

		#pragma region Paquete pesado
		case TutorialEvent::EntraPaquetePeso:

			DragAndDrop::enableDrag = false;
			scene_->createPackage(ecs::TutorialScene::BalanzaTut);
			delayedCallback(1, [this] {
				activateDialogue(false);
			delayedCallback(1, [this] {
				scene_->createBalanza();
				});
				});
			break;

		case TutorialEvent::EnviarPaquetePeso:
			DragAndDrop::enableDrag = false;
			activateDialogue(false);
			break;
		#pragma endregion

		#pragma region Paquete fragil
		case TutorialEvent::EntraPaqueteFragil:
			DragAndDrop::enableDrag = false;
			waitingWrapComp = scene_->createPackage(ecs::TutorialScene::Fragil)->getComponent<Wrap>();
			delayedCallback(1, [this] {
				activateDialogue(false);
				delayedCallback(1, [this] {
					scene_->createCinta();
					});
				});
			break;

		case TutorialEvent::SellarFragil:
			DragAndDrop::enableDrag = false;
			activateDialogue(false);
			break;

		case TutorialEvent::EnviarFragil:
			DragAndDrop::enableDrag = false;
			activateDialogue(false);
			break;
		#pragma endregion

		

		case TutorialEvent::Fin:
			activateDialogue(false);
			break;

	}
}

void TutorialSystem::stopEvent(TutorialEvent event) {

	switch (event) 
	{
		#pragma region Manual
		case TutorialEvent::Introduction:
			delayedCallback(1, [this]() {
				activateEvent(TutorialEvent::SacaElManual1);
				});
			break;
		case TutorialEvent::SacaElManual1:
			DragAndDrop::enableDrag = true;
			
			addActionListener(Action::SacarManual, [this]() {
				activateEvent(TutorialEvent::SacaElManual2);
				arrow_->removeComponent<MoverTransform>();
				arrow_->setActive(false);
				});
			break;
		case TutorialEvent::SacaElManual2:
				activateEvent(TutorialEvent::PaqueteEnsenarRemitente);
			break;
#pragma endregion

		#pragma region Primer Paquete
		case TutorialEvent::PaqueteEnsenarRemitente:
			delayedCallback(0.5, [this]() {
				activateEvent(TutorialEvent::PaqueteEnsenarCodigoPostal);
				});
			break;
		case TutorialEvent::PaqueteEnsenarCodigoPostal:
			delayedCallback(0.5, [this]() {
					activateEvent(TutorialEvent::PaqueteBuscarPaginaCodigosPostales);
				});
			break;
		case TutorialEvent::PaqueteBuscarPaginaCodigosPostales:
			arrow_->setActive(true);
			arrow_->getComponent<Transform>()->setRotation(320);

			arrow_->getComponent<Transform>()->setPos(
				scene_->getManualTransform()->getPos().getX() + (scene_->getManualTransform()->getWidth() * 0.93),
				scene_->getManualTransform()->getPos().getY() + (scene_->getManualTransform()->getHeigth() * 0.85));
			
			addActionListener(Action::PaginaCodigosPostales, [this]() {
				DragAndDrop::enableDrag = false;
				canPassPagesManual = false;
				activateEvent(TutorialEvent::BuscarPaginaHestia);
				});
			break;
		case TutorialEvent::BuscarPaginaHestia:
			canPassPagesManual = true;
			addActionListener(Action::PaginaDistritoHestia, [this]() {
				activateEvent(TutorialEvent::EnsenarSellos);
				});
			break;
		case TutorialEvent::EnsenarSellos:
			arrow_->setActive(false);
			DragAndDrop::enableDrag = true;
			addActionListener(Action::PaqueteEstampado, [this]() {
				activateEvent(TutorialEvent::EnsenarTubos);
				});
			break;
		case TutorialEvent::EnsenarTubos:
			canDrag = true;
			scene_->activateTubos();
			addActionListener(Action::PaqueteEnviado, [this]() {
				activateEvent(TutorialEvent::EntraSegundoPaquete);
				});
			break;
#pragma endregion

		#pragma region Segundo Paquete
		case TutorialEvent::EntraSegundoPaquete:
			addActionListener(Action::PaginaCodigosPostales, [this]() {
				activateEvent(TutorialEvent::SegundoBuscarPaginaDistritos);
				});
			break;
		case TutorialEvent::SegundoBuscarPaginaDistritos:
			addActionListener(Action::PaginaDistritoDemeter, [this]() {
				activateEvent(TutorialEvent::SellarSegundoPaquete);
				});
			break;
		case TutorialEvent::SellarSegundoPaquete:
			addActionListener(Action::PaqueteEstampado, [this]() {
				activateEvent(TutorialEvent::EnviarSegundoPaquete);
				});
			DragAndDrop::enableDrag = true;
			break;
		case TutorialEvent::EnviarSegundoPaquete:
			scene_->activateTubos();
			addActionListener(Action::PaqueteEnviado, [this]() {
				activateEvent(TutorialEvent::EntraCuartoPaquete);
				});
			break;
#pragma endregion

#pragma region Paquete Fallar Aposta
		case TutorialEvent::EntraCuartoPaquete:
			DragAndDrop::enableDrag = true;
			addActionListener(Action::Basura, [this] {
				activateEvent(TutorialEvent::ExplicacionFalloAposta);
				});
			break;
		case TutorialEvent::ExplicacionFalloAposta:
			delayedCallback(1, [this] {
				activateEvent(TutorialEvent::Fin);
				});
			break;
#pragma endregion

		#pragma region Tercer Paquete
		case TutorialEvent::EntraTercerPaquete:
			canPassPagesManual = true;
			addActionListener(Action::PaginaSellos, [this] {
				activateEvent(TutorialEvent::EnPaginaInfoSellos);
				});
			break;
		case TutorialEvent::EnPaginaInfoSellos:
			DragAndDrop::enableDrag = true;
			scene_->activateGarbage();
			addActionListener(Action::Basura, [this] {
				activateEvent(TutorialEvent::Fin);
				});
			break;
#pragma endregion



#pragma region Paquete Peso
		case TutorialEvent::EntraPaquetePeso:

			DragAndDrop::enableDrag = true;
			canPassPagesManual = true;
			waitingPesado = true;
			scene_->deactivateTubos();
			scene_->deactivateGarbage();

			arrow_->setActive(true);

			// animacion de la flecha
			arrow_->getComponent<Transform>()->setPos(1350, 625);
			arrow_->getComponent<Transform>()->setRotation(130);
			arrow_->addComponent<MoverTransform>(Vector2D(1150, 425), 1, Easing::EaseOutBack);
			arrow_->getComponent<MoverTransform>()->enable();

			addActionListener(Action::Pesado, [this]() {
				delayedCallback(1, [this] {
					activateEvent(TutorialSystem::EnviarPaquetePeso);
			arrow_->setActive(false);
					});
				});

			break;

		case TutorialEvent::EnviarPaquetePeso:

			DragAndDrop::enableDrag = true;
			canPassPagesManual = true;
			scene_->activateGarbage();
			scene_->activateTubos();
			addActionListener(Action::PaqueteEnviado, [this] {
				delayedCallback(1, [this] {
					activateEvent(TutorialSystem::Fin);
					});
				});
			break;
#pragma endregion

#pragma region Paquete Fragil
		case TutorialEvent::EntraPaqueteFragil:

			DragAndDrop::enableDrag = true;
			canPassPagesManual = true;
			scene_->activateTubos();
			addActionListener(Action::PaqueteEstampado, [this] {
				delayedCallback(1, [this] {
					activateEvent(TutorialSystem::SellarFragil);
					});
			});

			break;

		case TutorialEvent::SellarFragil:
			DragAndDrop::enableDrag = true;
			waitingEmbalaje = true;
			addActionListener(Action::Embalado, [this]() {
				delayedCallback(1, [this] {
					activateEvent(TutorialSystem::EnviarFragil);
					});
				});
			break;

		case TutorialEvent::EnviarFragil:
			DragAndDrop::enableDrag = true;
			waitingEmbalaje = true;
			addActionListener(Action::PaqueteEnviado, [this]() {
				delayedCallback(1, [this] {
					activateEvent(TutorialSystem::Fin);
					});
				});
			break;
#pragma endregion

		case TutorialEvent::Fin:
		
			gm().requestChangeScene(ecs::sc::TUTORIAL_SCENE, ecs::sc::MAIN_SCENE);
			// MANDAR A OTRA ESCENA O LO QUE QUERAMOS HACER

			break;
	}
}

void TutorialSystem::init()
{

	if (generalData().getDay() == 1) {

		tutorialIteration = Introduction;

	}
	else if (generalData().getDay() == 3) {

		tutorialIteration = EntraTercerPaquete;

	}
	else if (generalData().getDay() == 5) {

		tutorialIteration = EntraPaquetePeso;

	}
	else if (generalData().getDay() == 8) {

		tutorialIteration = EntraPaqueteFragil;

	}
	tutorialIteration = 0;
	DragAndDrop::enableDrag = true;
	waitingCallback = false;
	waitingEmbalaje = false;
	waitingPesado = false;

	dialogMngr_.init(scene_);
	createArrow();

}

void TutorialSystem::createArrow() {
	arrow_ = scene_->addEntity(ecs::layer::UI);
	arrow_->addComponent<Transform>(100, 100, 50, 100, 90);
	arrow_->addComponent<RenderImage>(&sdlutils().images().at("manecillaL"));

	arrow_->setActive(false);
}

void TutorialSystem::activateDialogue(bool dialogBoxInBottom) {
	dialogMngr_.setDialogueEntitiesActive(true);
	dialogMngr_.setDialogues(DialogManager::Tutorial, std::to_string(tutorialIteration));
	dialogMngr_.setEndDialogueCallback([this]{
		stopEvent(currentEvent);
	});
}

void TutorialSystem::delayedCallback(float time, SimpleCallback call) {
	call_ = call;

	waitingCallback = true;
	timeToCall_ = sdlutils().virtualTimer().currTime() + (time * 1000);
}

void TutorialSystem::registerAction(Action a)
{
	std::vector<std::vector<std::pair<Action, SimpleCallback>>::iterator> deleteUsedActionListeners;
	
	std::cout << a << std::endl;
	
	if (actionListeners.size() > 0)
	{
		for (auto it = actionListeners.begin(); it < actionListeners.end(); ++it)
		{
			if ((*it).first == a)
			{
				(*it).second();
				deleteUsedActionListeners.push_back(it);
			}
		}
	}

	for (auto it : deleteUsedActionListeners)
		actionListeners.erase(it);
}

void TutorialSystem::addActionListener(Action a, SimpleCallback call)
{
	actionListeners.push_back(std::make_pair(a, call));
}