#pragma once

#include <list>
#include <functional>

#include <architecture/Component.h>

class Transform;

using Callback = std::function<void(ecs::Entity*)>;



// Trigger permite interactuar con objetos que tengan dragAndDrop
// si se le arrastra encima un objeto con dragAndDrop se activarán los
// callbacks del trigger del objeto de abajo.
// Para añadir callbacks usamos addCallback
class Trigger : public ecs::Component {


public:

	__CMP_DECL__(ecs::cmp::TRIGGER)

	Trigger(std::list<ecs::Entity*>::iterator it);

	~Trigger();

	void initComponent() override;

	void update() override;

	//Se registra cuando se ha tocado una entidad y se agrega a la lista de entidades
	void touchEntity(ecs::Entity* ent);

	//Se agrega un callback el cual sera llamado con los metodos pertinentes
	void addCallback(Callback event, int moveType);

	//Elimina todos los callbacks que haya actualmente
	void clearCallback(int moveType);

	//activa los callbacks de los triggers con los que este tocando el trigger
	bool activateEventsFromEntities(int moveType);

	//activa los callbacks de la entidad mas cercana con la que este tocando el trigger
	bool activateEventFromClosestEntity(int moveType);

	//activa los callbacks del propio trigger
	bool activateCallbacks(ecs::Entity*, int moveType);

	//comprueba si esta entidad es la mas cercana
	bool checkIfClosest();

	//devuelve una entidad especifica dada por su layer
	ecs::Entity* getSpecificEntity(ecs::layer::layerId lay);

	//devuelve una lista con todas las entidades que esta tocando
	std::list <ecs::layer::layerId> getEntitiesTouching();

private:

	Transform* mTr_;

	std::list <ecs::Entity*> entTouching_;

	//lista de eventos que se llaman cuando se deja caer algo en este objeto
	std::list <Callback> eventList_;

	//lista de eventos que se llaman cuando se recoge algo en este objeto
	std::list <Callback> eventListPickUp_;

	std::list<ecs::Entity*>::iterator collisionIterator_;


};

