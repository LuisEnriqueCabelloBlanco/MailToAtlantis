#include "TutorialSystem.h"
#include "../components/Render.h"
#include "../scenes/TutorialScene.h"
#include "../components/DialogManager.h"
#include "../components/DialogComponent.h"

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
			call_();
			waitingCallback = false;
		}
	}
}

void TutorialSystem::activateEvent(TutorialEvent event) {

	currentEvent = event;
	tutorialIteration = (int)event;
	switch (event)
	{
		case TutorialEvent::Introduction:
			canDrag = false;
			activateDialogue(DialogManager::Tutorial);
			break;
		case TutorialEvent::SacaElManual:
			canDrag = false;
			activateDialogue(DialogManager::Tutorial);
			arrow_->setActive(true);
			arrow_->getComponent<Transform>()->setPos(600,600);
			arrow_->getComponent<Transform>()->setRotation(130);
			break;
	}
}

void TutorialSystem::stopEvent(TutorialEvent event) {

	switch (event) {
		case TutorialEvent::Introduction:
			delayedCallback(1, [this]() {
				activateEvent(TutorialEvent::SacaElManual);
				});
			break;
		case TutorialEvent::SacaElManual:
			canDrag = true;
			arrow_->setActive(false);
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