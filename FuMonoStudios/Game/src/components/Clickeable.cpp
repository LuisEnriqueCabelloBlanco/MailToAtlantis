#include <sdlutils/InputHandler.h>
#include <components/Clickeable.h>

#include <components/Transform.h>
#include <components/Trigger.h>
#include <architecture/Entity.h>
#include <SDL.h>
#include <assert.h>

#include <sistemas/SoundEmiter.h>


Clickeable::Clickeable(std::string soundClick): mTr_(nullptr), eventsWhenClick_(), soundWhenClicked_(soundClick), canClick_(true) {


}

Clickeable::~Clickeable() {

}

void Clickeable::initComponent() {

	mTr_ = ent_->getComponent<Transform>();

	//Evitar poner clickeable si ya hay trigger
	Trigger* tri_ = ent_->getComponent<Trigger>();

	assert(mTr_ != nullptr);

	assert(tri_ == nullptr);

}
	

// cleon: "de hecho, no tenemos delta"
void Clickeable::update() {

	auto& ihdlr = ih();

	if (ihdlr.mouseButtonDownEvent() && canClick_) {

		SDL_Rect* mRect_ = &mTr_->getRect();
		SDL_Point point{ ihdlr.getMousePos().first, ihdlr.getMousePos().second };

		if (SDL_PointInRect(&point, mRect_)) {
			// 
			//Recorrido por las callbacks a las que esta suscrito este objeto
			if (soundWhenClicked_ != "") {
				SoundEmiter::instance()->playSound(soundWhenClicked_);
			}
			for (CallbackClickeable call : eventsWhenClick_) {

				call();
			}
		}
	}
}

void Clickeable::addEvent(CallbackClickeable event) {
	eventsWhenClick_.push_back(event);
}
