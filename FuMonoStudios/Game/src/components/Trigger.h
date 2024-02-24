#pragma once

#include <list>
#include <functional>

#include "../architecture/Component.h"

class Transform;

using Callback = std::function<void(ecs::Entity*)>;

class Trigger : public ecs::Component {


public:

	__CMP_DECL__(ecs::cmp::TRIGGER)

	Trigger();

	~Trigger();

	void initComponent() override;

	void update() override;

	void touchEntity(ecs::Entity* ent);

	void addCallback(Callback event);

	bool activateEventsFromEntities();

	bool activateCallbacks(ecs::Entity*);

private:

	Transform* tr_;

	std::list <ecs::Entity*> entTouching;

	std::list <Callback> eventList;



};

