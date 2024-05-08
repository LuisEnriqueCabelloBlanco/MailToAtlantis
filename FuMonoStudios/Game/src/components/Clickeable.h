#pragma once

#include <architecture/Component.h>
#include <functional>
#include <list>
#include <string>

class Transform;

using CallbackClickeable = std::function<void(void)>;

class Clickeable : public ecs::Component {

public:
	__CMP_DECL__(ecs::cmp::CLICKEABLE)
	Clickeable(std::string soundClick);

	~Clickeable();

	void initComponent() override;

	void update() override;

	void addEvent(CallbackClickeable event );

	void toggleClick(bool onoff) { canClick_ = onoff; }
private:

	Transform* mTr_;

	std::list<CallbackClickeable> eventsWhenClick_;

	std::string soundWhenClicked_;

	bool canClick_;

};
