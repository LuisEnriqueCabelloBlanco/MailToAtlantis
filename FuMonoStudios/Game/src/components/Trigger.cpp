
#include "Trigger.h"

#include <components/Transform.h>
#include <components/Clickeable.h>
#include <architecture/GeneralData.h>
#include <architecture/Entity.h>

#include <assert.h>


Trigger::Trigger(std::list<ecs::Entity*>::iterator it) : mTr_(nullptr)
{
	collisionIterator_ = it;
}

Trigger::~Trigger() {
	ent_->getMngr()->removeCollison(collisionIterator_);
}

void Trigger::initComponent() {

	mTr_ = ent_->getComponent<Transform>();

	//asegurarse que si hay un trigger no hay un trigger clickeable
	Clickeable* cl_ = ent_->getComponent<Clickeable>();

	assert(mTr_ != nullptr);
	assert(cl_ == nullptr);

}

//se limpia la lista de colisiones y se comprueba en ese frame si se esta tocando con algo
void Trigger::update() {

	entTouching_.clear();

	if (ent_->getMngr()->checkColisions(ent_)) {

		//std::cout << "Choca << std::endl;

	}


}

//Se registra cuando se ha tocado a una entidad y se hace un push back en la lista de entidades en contacto
void Trigger::touchEntity(ecs::Entity* ent) {

	entTouching_.push_back(ent);

}

//Aï¿½ade funcionalidad a la entidad si algo se levanta sobre ella
void Trigger::addCallback(Callback event, int moveType) {

	if (moveType == gD().DropIn) {

		eventList_.push_back(event);

	}
	else if (moveType == gD().PickUp) {

		eventListPickUp_.push_back(event);

	}
	

}

void Trigger::clearCallback(int moveType)
{

	if (moveType == gD().DropIn) {

		eventList_.clear();

	}
	else if (moveType == gD().PickUp) {

		eventListPickUp_.clear();

	}

}

//activa los eventos de todas las entidades que tenga asociadas (que este tocando)
//NOTA: en un futuro serï¿½ necesario implementar un sistema de layers para diferenciar que cosa puede tocar a que cosa
bool Trigger::activateEventsFromEntities(int moveType) {

	for (auto it = entTouching_.begin(); it != entTouching_.end(); ++it) {

		if ((*it)->isActive()) {

			(*it)->getComponent<Trigger>()->activateCallbacks(ent_, moveType);

		}
		
	}

	return entTouching_.empty();

}

bool Trigger::activateEventFromClosestEntity(int moveType) {
	Trigger* closestEnt = nullptr;
	float shortestDistance = 9999999;
	Vector2D entPos = ent_->getComponent<Transform>()->getCenter();

	for (auto it = entTouching_.begin(); it != entTouching_.end(); ++it)
	{
		Vector2D otherPos = (*it)->getComponent<Transform>()->getCenter();
		float distance = sqrt(pow(otherPos.getX() - entPos.getX(), 2) + pow(otherPos.getY() - entPos.getY(), 2));
		if (distance < shortestDistance && (*it)->isActive())
		{
			shortestDistance = distance;
			closestEnt = (*it)->getComponent<Trigger>();
		}
	}

	// si ha encontrado una entidad al menos, despues de haber ciclado
	// por todas tenemos la mas cercana y llamamos solo a esa
	if (closestEnt != nullptr)
		closestEnt->activateCallbacks(ent_, moveType);

	return closestEnt != nullptr;
}

//Activa las funciones asociadas a esta entidad
bool Trigger::activateCallbacks(ecs::Entity* Ent, int moveType) {



	if(Ent != ent_){
	
		if (moveType == gD().DropIn) {

			for (Callback call : eventList_) {

				call(Ent);
			}

		}
		else if (moveType == gD().PickUp) {

			for (Callback call : eventListPickUp_) {

				call(Ent);
			}
		}
		
	}
	

	return eventList_.empty();

}

//Se comprueba si la entidad con este trigger esta más cercana a la pantalla que el resto de entidades con las que choca
bool Trigger::checkIfClosest() {

	auto it = entTouching_.begin();

	ecs::layer::layerId myLayer = ent_->getLayer();

	while (it != entTouching_.end() && (!(*it)->getComponent<Transform>()->getIfPointerIn() || myLayer >= (*it)->getLayer())) {

		++it;

	}

	return it == entTouching_.end();

}

ecs::Entity* Trigger::getSpecificEntity(ecs::layer::layerId lay) {

	if (!entTouching_.empty()) {

		auto it = entTouching_.begin();

		while (it != entTouching_.end() && (*it)->getLayer() != lay) {

			++it;

		}

		if (it != entTouching_.end()) {
			return (*it);
		}
		else {
			return nullptr;
		}
	}
	
	return nullptr;

}

std::list <ecs::layer::layerId> Trigger::getEntitiesTouching() {

	std::list <ecs::layer::layerId> entTouchingID;

	if (!entTouching_.empty()) {

		for (auto i = entTouching_.begin(); i != entTouching_.end(); ++i) {

			entTouchingID.emplace_back((*i)->getLayer());

		}

	}

	return entTouchingID;

}