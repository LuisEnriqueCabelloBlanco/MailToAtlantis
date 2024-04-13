#pragma once

#include <list>
#include <functional>

#include "../architecture/Component.h"

class Transform;

using Callback = std::function<void(ecs::Entity*)>;

///
/// MoveTypes
/// 0
///

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

	void touchEntity(ecs::Entity* ent);

	void addCallback(Callback event);

	void addCallbackPickUp(Callback event);

	bool activateEventsFromEntities(int moveType);

	bool activateEventFromClosestEntity(int moveType);

	bool activateCallbacks(ecs::Entity*);

	bool activateCallbacksPickUp(ecs::Entity*);

	bool checkIfClosest();

	ecs::Entity* getSpecificEntity(ecs::layer::layerId lay);

	std::list <ecs::layer::layerId> getEntitiesTouching();

private:

	Transform* mTr_;

	std::list <ecs::Entity*> entTouching_;

	std::list <Callback> eventList_;

	std::list <Callback> eventListPickUp_;

	std::list<ecs::Entity*>::iterator collisionIterator_;


};

