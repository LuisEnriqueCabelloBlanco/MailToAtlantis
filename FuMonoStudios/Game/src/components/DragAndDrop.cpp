
#include "DragAndDrop.h"

#include "Transform.h"
#include "Gravity.h"
#include "Trigger.h"
#include "../architecture/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "Gravity.h"

#include <SDL.h>
#include <assert.h>

DragAndDrop::DragAndDrop() : tr_(nullptr), tri_(nullptr), grav_(nullptr), dragging(false), differenceX(0), differenceY(0),
usingCallback(false), usingOnlyClosestEnt(false) {
}


DragAndDrop::DragAndDrop(bool UsingOnlyClosestEnt) : 
	tr_(nullptr), tri_(nullptr), grav_(nullptr), dragging(false), differenceX(0), differenceY(0), 
	usingCallback(false), usingOnlyClosestEnt(UsingOnlyClosestEnt) {
}

DragAndDrop::DragAndDrop(bool UsingOnlyClosestEnt, SimpleCallback Func) : 
	tr_(nullptr), tri_(nullptr), grav_(nullptr), dragging(false), differenceX(0), differenceY(0),
	usingCallback(true), usingOnlyClosestEnt(UsingOnlyClosestEnt)
{
	func = Func;
}

DragAndDrop::~DragAndDrop() {

}

void DragAndDrop::initComponent() {
	tri_ = ent_->getComponent<Trigger>();

	if (tri_ != nullptr)
		throw std::runtime_error("Entidad con trigger asignado DragAndDrop (el dragnDrop lo asigna automaticamente))");

	tri_ = ent_->addComponent<Trigger>();

	tr_ = ent_->getComponent<Transform>();

	grav_ = ent_->getComponent<Gravity>();

	assert(tr_ != nullptr);

}

void DragAndDrop::update() {
	auto& ihdlr = ih();

	SDL_Point point{ ihdlr.getMousePos().first, ihdlr.getMousePos().second };

	//Deteccion al clicar sobre el objeto
	if (ihdlr.mouseButtonDownEvent()) {
		if (tr_->getIfPointerIn() && tri_->checkIfClosest()) {

			dragging = true;
			// desactivamos gravedad al draggear
			if (grav_ != nullptr) {
				grav_->setActive(false);
			}

			//Para que funcione sin ir al centro, con margen
			differenceX = point.x - tr_->getPos().getX();;
			differenceY = point.y - tr_->getPos().getY();;
		}
	}
	//Deteccion al soltar el objeto
	else if (ihdlr.mouseButtonUpEvent()) {

		dragging = false;
		// reactivamos la gravedad
		if (grav_ != nullptr) {
			grav_->setActive(true);
		}

		//Al soltar el objeto activa los callback de todas las entidades que este tocando el objeto
		// si no tenemos activado el activar solo al mas cercano
		if (!usingOnlyClosestEnt)
			tri_->activateEventsFromEntities();
		else
			tri_->activateEventFromClosestEntity();

		// si has asignado callback se activa
		if (usingCallback)
			func();
	}

	//Arrastre del objeto
	if (dragging) {
		//Para que vaya en el medio
		//tr_->setPos(point.x - (tr_->getWidth() / 2), point.y - (tr_->getHeith() / 2));
		
		// comprobacion para evitar sacar la entidad de la pantalla
		if ((point.x - differenceX > -(tr_->getWidth() / 2))
			&& (point.x - differenceX < sdlutils().width() - (tr_->getWidth() / 2)) 
			&& (point.y - differenceY < sdlutils().height() - (tr_->getHeigth() / 6)))
		{
			//Sin centrarse el objeto
			tr_->setPos(point.x - differenceX, point.y - differenceY);
		}
	}
}