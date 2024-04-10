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
	HoverSensorComponent(std::function<void()> hoverCall);
	~HoverSensorComponent();

	virtual void update() override;

	void addCall(std::function<void()> hoverCall);

private:
	Transform* mTr_;
	std::list<std::function<void()>> hoverCall_;
};

