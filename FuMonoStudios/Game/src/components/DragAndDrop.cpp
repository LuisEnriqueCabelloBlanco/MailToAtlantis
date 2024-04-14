
#include "DragAndDrop.h"

#include "Transform.h"
#include "Gravity.h"
#include "Trigger.h"
#include "../architecture/Entity.h"
#include "../sdlutils/InputHandler.h"
#include <architecture/GameConstants.h>
#include "../architecture/GeneralData.h"
#include "../sistemas/SoundEmiter.h"

#include <SDL.h>
#include <assert.h>

DragAndDrop::DragAndDrop() : tr_(nullptr), tri_(nullptr), grav_(nullptr), dragging_(false), differenceX_(0), differenceY_(0),
usingCallback_(false), usingOnlyClosestEnt_(false), usingOwnCallback_(false)
{
}

DragAndDrop::DragAndDrop(std::string sound) : tr_(nullptr), tri_(nullptr), grav_(nullptr), dragging_(false), differenceX_(0), differenceY_(0),
usingCallback_(false), usingOnlyClosestEnt_(false), usingOwnCallback_(false), draggingSound_(sound)
{
}


DragAndDrop::DragAndDrop(bool UsingOnlyClosestEnt, std::string sound) :
	tr_(nullptr), tri_(nullptr), grav_(nullptr), dragging_(false), differenceX_(0), differenceY_(0), 
	usingCallback_(false), usingOnlyClosestEnt_(UsingOnlyClosestEnt), usingOwnCallback_(false), 
	draggingSound_(sound){
}

DragAndDrop::DragAndDrop(bool usingClosestEnt, bool usingOwnCallback, std::string sound) : tr_(nullptr), tri_(nullptr), grav_(nullptr), dragging_(false), differenceX_(0), differenceY_(0),
usingCallback_(false), usingOnlyClosestEnt_(usingClosestEnt), usingOwnCallback_(usingOwnCallback),
draggingSound_(sound) {
}

DragAndDrop::DragAndDrop(bool UsingOnlyClosestEnt, SimpleCallback Func, std::string sound) :
	tr_(nullptr), tri_(nullptr), grav_(nullptr), dragging_(false), differenceX_(0), differenceY_(0),
	usingCallback_(true), usingOnlyClosestEnt_(UsingOnlyClosestEnt), usingOwnCallback_(false),
	draggingSound_(sound)
{
	func_ = Func;
}

DragAndDrop::~DragAndDrop() {
	sdlutils().soundEffects().at(draggingSound_ + std::to_string(0)).haltChannel();
}

void DragAndDrop::initComponent() {
	assert(ent_->getComponent<Trigger>() == nullptr);

	tri_ = ent_->addComponent<Trigger>();

	tr_ = ent_->getComponent<Transform>();

	grav_ = ent_->getComponent<Gravity>();

	assert(tr_ != nullptr);

	latestPoint_ = std::pair<int,int>(0,0);
}

void DragAndDrop::update() {
	auto& ihdlr = ih();

	SDL_Point point{ ihdlr.getMousePos().first, ihdlr.getMousePos().second };

	if (canInteract)
	{
		//Deteccion al clicar sobre el objeto
		if (ihdlr.mouseButtonDownEvent()) {
			if (tr_->getIfPointerIn() && tri_->checkIfClosest()) {
				if (!dragging_)
					SoundEmiter::instance()->playSound(draggingSound_);

				dragging_ = true;

				if (usingOwnCallback_) {
					tri_->activateCallbacks(nullptr, generalData().PickUp);
				}
				else {

					if (!usingOnlyClosestEnt_)
						tri_->activateEventsFromEntities(generalData().PickUp);
					else
						tri_->activateEventFromClosestEntity(generalData().PickUp);

				}

				// desactivamos gravedad al draggear
				if (grav_ != nullptr) {
					grav_->setActive(false);
				}

				//Para que funcione sin ir al centro, con margen
				differenceX_ = point.x - tr_->getPos().getX();
				differenceY_ = point.y - tr_->getPos().getY();
				porcentajeStart = tr_->getPorcentajeScale();
			}
		}
		//Deteccion al soltar el objeto
		else if (ihdlr.mouseButtonUpEvent()) {
			
			if (dragging_) {
				// reactivamos la gravedad
				if (grav_ != nullptr) {
					grav_->setActive(true);
				}

				//Al soltar el objeto activa los callback de todas las entidades que este tocando el objeto
				// si no tenemos activado el activar solo al mas cercano

				if (usingOwnCallback_) {
					tri_->activateCallbacks(nullptr, generalData().DropIn);
				}
				else {

					if (!usingOnlyClosestEnt_)
						tri_->activateEventsFromEntities(generalData().DropIn);
					else
						tri_->activateEventFromClosestEntity(generalData().DropIn);

				}

				SoundEmiter::instance()->haltSound(draggingSound_);
				// si has asignado callback se activa
				if (usingCallback_)
					func_();
			}
			

			dragging_ = false;
		}

		//Arrastre del objeto
		if (dragging_) {

			if (latestPoint_.first != point.x || latestPoint_.second != point.y)
				SoundEmiter::instance()->muteSingleSound(draggingSound_, false);
			else
				SoundEmiter::instance()->muteSingleSound(draggingSound_, true);


			latestPoint_.first = point.x;
			latestPoint_.second = point.y;
			//Para que vaya en el medio
			//tr_->setPos(point.x - (tr_->getWidth() / 2), point.y - (tr_->getHeigth() / 2));

			// comprobacion para evitar sacar la entidad de la pantalla
			if ((point.x - differenceX_ > -(tr_->getWidth() / 2))
				&& (point.x - differenceX_ < LOGICAL_RENDER_WIDTH - (tr_->getWidth() / 2))
				&& (point.y - differenceY_ < LOGICAL_RENDER_HEITH - (tr_->getHeigth() / 6)))
			{

				Vector2D pos = Vector2D(
					point.x - (differenceX_ * (1 - (porcentajeStart - tr_->getPorcentajeScale()))),
					point.y - (differenceY_ * (1 - (porcentajeStart - tr_->getPorcentajeScale()))));

				//Sin centrarse el objeto
				//tr_->setPos((point.x - differenceX_), (point.y - differenceY_));
				tr_->setPos(pos);

			}
		}
	}
}