
#include "Trigger.h"

#include "Transform.h"
#include "Clickeable.h"
#include "../architecture/Entity.h"

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

//Aï¿½ade funcionalidad a la entidad si algo se posa sobre ella
void Trigger::addCallback(Callback event) {

	eventList_.push_back(event);

}

//activa los eventos de todas las entidades que tenga asociadas (que este tocando)
//NOTA: en un futuro serï¿½ necesario implementar un sistema de layers para diferenciar que cosa puede tocar a que cosa
bool Trigger::activateEventsFromEntities() {

	for (auto it = entTouching_.begin(); it != entTouching_.end(); ++it) {

		(*it)->getComponent<Trigger>()->activateCallbacks(ent_);

	}

	return entTouching_.empty();

}

bool Trigger::activateEventFromClosestEntity() {
	Trigger* closestEnt = nullptr;
	float shortestDistance = 9999999;
	Vector2D entPos = ent_->getComponent<Transform>()->getCenter();

	for (auto it = entTouching_.begin(); it != entTouching_.end(); ++it)
	{
		Vector2D otherPos = (*it)->getComponent<Transform>()->getCenter();
		float distance = sqrt(pow(otherPos.getX() - entPos.getX(), 2) + pow(otherPos.getY() - entPos.getY(), 2));
		if (distance < shortestDistance)
		{
			shortestDistance = distance;
			closestEnt = (*it)->getComponent<Trigger>();
		}
	}

	// si ha encontrado una entidad al menos, despues de haber ciclado
	// por todas tenemos la mas cercana y llamamos solo a esa
	if (closestEnt != nullptr)
		closestEnt->activateCallbacks(ent_);

	return closestEnt != nullptr;
}

//Activa las funciones asociadas a esta entidad
bool Trigger::activateCallbacks(ecs::Entity* Ent) {


	for (Callback call : eventList_) {

		call(Ent);
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