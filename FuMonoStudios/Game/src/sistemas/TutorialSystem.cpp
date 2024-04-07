#include "TutorialSystem.h"
#include "../components/Render.h"
#include "../scenes/TutorialScene.h"
#include "../components/DialogManager.h"
#include "../components/DialogComponent.h"
#include "../components/MoverTransform.h"

TutorialSystem::TutorialSystem(ecs::TutorialScene* scene) {
	scene_ = scene;
	tutorialIteration = 0;
	canDrag = true;
	waitingCallback = false;


	createDialogueBox();
	createArrow();
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
}

void TutorialSystem::activateEvent(TutorialEvent event) {

	currentEvent = event;
	tutorialIteration = (int)event;
	switch (event)
	{
		case TutorialEvent::Introduction:
			canPassPagesManual = false;
			canDrag = false;
			activateDialogue(DialogManager::Tutorial);
			break;
		case TutorialEvent::SacaElManual1:
			canDrag = false;
			activateDialogue(DialogManager::Tutorial);
			arrow_->setActive(true);
			
			// animacion de la flecha
			arrow_->getComponent<Transform>()->setPos(1150,410);
			arrow_->getComponent<Transform>()->setRotation(130);
			arrow_->addComponent<MoverTransform>(Vector2D(800, 700), 1, Easing::EaseOutBack);
			arrow_->getComponent<MoverTransform>()->enable();

			delayedCallback(1.2, [this]() {
				arrow_->getComponent<Transform>()->setPos(1150, 410);
				arrow_->addComponent<MoverTransform>(Vector2D(800, 700), 1, Easing::EaseOutBack, [this]() {
					arrow_->getComponent<Transform>()->setPos(1150, 410);
					});
				arrow_->getComponent<MoverTransform>()->enable();
				});
			break;
		case TutorialEvent::SacaElManual2:
			canDrag = false;
			activateDialogue(DialogManager::Tutorial);
			break;
		case TutorialEvent::PaqueteEnseñarRemitente:
			canDrag = false;
			scene_->createPackage(1);
			delayedCallback(1, [this]() {
				activateDialogue(DialogManager::Tutorial);
				delayedCallback(1, [this]() {
					arrow_->setActive(true);
					arrow_->getComponent<Transform>()->setPos(1340, 720);
					});
				});
			break;
		case TutorialEvent::PaqueteEnseñarCodigoPostal:
			activateDialogue(DialogManager::Tutorial);
			arrow_->getComponent<Transform>()->setPos(1340, 680);
			break;
		case TutorialEvent::PaqueteBuscarPaginaCodigosPostales:
			canPassPagesManual = true;
			activateDialogue(DialogManager::Tutorial);
			break;
		case TutorialEvent::PaqueteBuscarPaginaHestia:
			canPassPagesManual = false;
			activateDialogue(DialogManager::Tutorial);
			break;
		case TutorialEvent::PaqueteEnseñarSellos:
			canPassPagesManual = false;
			activateDialogue(DialogManager::Tutorial);
			break;
	}
}

void TutorialSystem::stopEvent(TutorialEvent event) {

	switch (event) {
		case TutorialEvent::Introduction:
			delayedCallback(1, [this]() {
				activateEvent(TutorialEvent::SacaElManual1);
				});
			break;
		case TutorialEvent::SacaElManual1:
			canDrag = true;
			
			addActionListener(Action::SacarManual, [this]() {
				activateEvent(TutorialEvent::SacaElManual2);
				arrow_->removeComponent<MoverTransform>();
				arrow_->setActive(false);
				});
			break;
		case TutorialEvent::SacaElManual2:
				activateEvent(TutorialEvent::PaqueteEnseñarRemitente);
			break;
		case TutorialEvent::PaqueteEnseñarRemitente:
			delayedCallback(0.5, [this]() {
				activateEvent(TutorialEvent::PaqueteEnseñarCodigoPostal);
				});
			break;
		case TutorialEvent::PaqueteEnseñarCodigoPostal:
			delayedCallback(0.5, [this]() {
					activateEvent(TutorialEvent::PaqueteBuscarPaginaCodigosPostales);
				});
			break;
		case TutorialEvent::PaqueteBuscarPaginaCodigosPostales:
			arrow_->setActive(false);
			addActionListener(Action::PaginaCodigosPostales, [this]() {
				canDrag = false;
				canPassPagesManual = false;
				activateEvent(TutorialEvent::PaqueteBuscarPaginaHestia);
				});
			break;
		case TutorialEvent::PaqueteBuscarPaginaHestia:
			canPassPagesManual = true;
			addActionListener(Action::PaginaDistritoHestia, [this]() {
				activateEvent(TutorialEvent::PaqueteEnseñarSellos);
				});
			break;
	}
}

void TutorialSystem::closeConversation() {
	stopEvent(currentEvent);

	textDialogue_->getComponent<RenderImage>()->setTexture(nullptr);
	textDialogue_->removeComponent<DialogComponent>();
	boxBackground_->getComponent<RenderImage>()->setTexture(nullptr);
}

void TutorialSystem::createDialogueBox() {

	// creamos la entidad caja dialogo
	boxBackground_ = scene_->addEntity(ecs::layer::UI);
	auto bgTr = boxBackground_->addComponent<Transform>(100, LOGICAL_RENDER_HEITH - 250, LOGICAL_RENDER_WIDTH - 100, 200);
	boxBackground_->addComponent<RenderImage>(&sdlutils().images().at("cuadroDialogo"));

	// entidad del texto
	textDialogue_ = scene_->addEntity(ecs::layer::UI);
	auto textTr = textDialogue_->addComponent<Transform>(100, 40, 80, 100);
	textTr->setParent(bgTr);
	textDialogue_->addComponent<RenderImage>();

	textDialogue_->getComponent<RenderImage>()->setTexture(nullptr);
	boxBackground_->getComponent<RenderImage>()->setTexture(nullptr);
}

void TutorialSystem::createArrow() {
	arrow_ = scene_->addEntity(ecs::layer::UI);
	arrow_->addComponent<Transform>(100, 100, 50, 100, 90);
	arrow_->addComponent<RenderImage>(&sdlutils().images().at("manecillaL"));

	arrow_->setActive(false);
}

void TutorialSystem::activateDialogue(DialogManager::DialogSelection ds) {
	boxBackground_->getComponent<RenderImage>()->setTexture(&sdlutils().images().at("cuadroDialogo"));

	dialogMngr_.setDialogues(ds, std::to_string(tutorialIteration));
	textDialogue_->addComponent<DialogComponent>(&dialogMngr_, scene_);
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