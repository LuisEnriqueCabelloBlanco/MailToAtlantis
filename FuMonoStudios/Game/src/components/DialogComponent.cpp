#include "../sdlutils/InputHandler.h"
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "DialogComponent.h"
#include "../architecture/Entity.h"
#include "../sdlutils/Font.h"
#include "../sdlutils/Texture.h"
#include "DialogManager.h"
#include "Transform.h"
#include "Render.h"
#include "DelayedCallback.h"
#include "../scenes/ExplorationScene.h"
#include "../scenes/TutorialScene.h"

DialogComponent::DialogComponent(DialogManager* manager): 
	mTr_(nullptr), mRend_(nullptr),
	dialogueWidth_(sdlutils().width() + 500),dialogueIndex_(1),mTexture_(nullptr),
	canSkip(true), endDialogue(false)
{
	mDialogMngr_ = manager;
	mFont_ = new Font("recursos/fonts/ARIAL.ttf", 40);
}


DialogComponent::~DialogComponent()
{
	delete mFont_;
	delete mTexture_;
}

void DialogComponent::initComponent()
{
	mTr_ = ent_->getComponent<Transform>();
	assert(mTr_ != nullptr);
	mRend_ = ent_->getComponent<RenderImage>();
	assert(mRend_ != nullptr);
	setCurrentDialogue();
}

void DialogComponent::update()
{
	//Escritura caracter a caracter
	if (sdlutils().virtualTimer().currTime() > lastTimePaused_ + 40) { // este 40 en mejor sitio
		setCurrentDialogue();
		//avance al siguiente caracter
		if (dialogueIndex_ < mDialogMngr_->getCurrentDialog().size())
			dialogueIndex_++;

		lastTimePaused_ = sdlutils().virtualTimer().currTime();
	}

	//al pulsar espacio o cualquier boton del ratón
	if (canSkip &&
		(ih().isKeyDown(SDL_SCANCODE_SPACE) || ih().mouseButtonDownEvent()))
	{
		// cooldown
		canSkip = false;
		ent_->addComponent<DelayedCallback>(0.2, [this]() {
			canSkip = true;
			});

		// Pasar al siguiente dialogo o terminar conversacion
		if (dialogueIndex_ == mDialogMngr_->getCurrentDialog().size())
		{
			endDialogue = mDialogMngr_->nextDialog();
			dialogueIndex_ = 1;

			if (endDialogue)
			{
				if (mTexture_ != nullptr)
				{
					mDialogMngr_->closeDialogue();
				}
			}
		}
		//Sacar todo el diálogo antes de que acabe de escribirse
		else
		{
			dialogueIndex_ = mDialogMngr_->getCurrentDialog().size();
		}
	}

}

void DialogComponent::setCurrentDialogue()
{
	if (mTexture_ != nullptr) {
		delete mTexture_;
		mTexture_ = nullptr;
	}
	if(mDialogMngr_->getCurrentDialog().substr(0, dialogueIndex_) != "")
	{
		mTexture_ = new Texture(sdlutils().renderer(), mDialogMngr_->getCurrentDialog().substr(0, dialogueIndex_),
			*mFont_, build_sdlcolor(0xffffffffff), dialogueWidth_);
		mRend_->setTexture(mTexture_);
		mTr_->setWidth(mTexture_->width());
		mTr_->setHeith(mTexture_->height());
	}

}