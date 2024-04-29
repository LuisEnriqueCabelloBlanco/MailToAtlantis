#pragma once
#include <list>
#include <functional>

#include "../architecture/Component.h"
class Transform;

class HoverSensorComponent:public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::HOVERSENSOR)

	HoverSensorComponent();
	~HoverSensorComponent(); 


	virtual void update() override;

	void initComponent() override;
	void addInCall(std::function<void()> hoverCall);
	void addOutCall(std::function<void()> hoverCall);
	void addDestoryCall(std::function<void()> destroyCall);

private:
	void activateInCalls();
	void activateOutCalls();

	bool inside_;
	Transform* mTr_;
	std::list<std::function<void()>> inCalls_;
	std::list<std::function<void()>> outCalls_;
	std::list<std::function<void()>> destroyCalls_;
};

