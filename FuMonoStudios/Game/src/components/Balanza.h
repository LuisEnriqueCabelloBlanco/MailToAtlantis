#pragma once
#include "../architecture/Component.h"
class Balanza: public ecs::Component
{

public:
	__CMP_DECL__(ecs::cmp::BALANZA)
		Balanza();

	~Balanza();

	void initComponent() override;

	void initAnimations();
	void finishAnimatios();

	void setFlecha(Entity* flecha) { flecha_ = flecha; }

private:
	Entity* flecha_;
	bool startAnimation;
};

