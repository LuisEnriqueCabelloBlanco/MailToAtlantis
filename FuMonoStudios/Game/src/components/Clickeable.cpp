#include "Clickeable.h"

#include "Transform.h"
#include "../architecture/Entity.h"
#include "../sdlutils/InputHandler.h"

#include <SDL.h>
#include <assert.h>



Clickeable::Clickeable(): tr_(nullptr), eventsWhenClick() {

}

Clickeable::~Clickeable() {

}

void Clickeable::initComponent() {

	tr_ = ent_->getComponent<Transform>();

	assert(tr_ != nullptr);

}
	

// cleon: "de hecho, no tenemos delta"
void Clickeable::update() {

	auto& ihdlr = ih();

	if (ihdlr.mouseButtonDownEvent()) {

		SDL_Point point{ ihdlr.getMousePos().first, ihdlr.getMousePos().second};

		if (SDL_PointInRect(&point, &tr_->getRect())) {

			std::cout << "click" << std::endl;
			// 
			//Recorrido por las callbacks a las que esta suscrito este objeto
			for (Callback call:eventsWhenClick) {

				call(ent_);
			}

		}

	}


}

void Clickeable::addEvent(Callback event) {

	eventsWhenClick.push_back(event);

}
