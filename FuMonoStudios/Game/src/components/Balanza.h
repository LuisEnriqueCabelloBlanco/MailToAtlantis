#pragma once
#include "../architecture/Component.h"

class RotarTransform;

class Balanza: public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::BALANZA)
		Balanza();

	~Balanza();

	void initComponent() override;

	void initAnimations(ecs::Entity* paquete, RotarTransform* flechaRotComp);
	void finishAnimatios(RotarTransform* flechaRotComp);

	void setFlecha(Entity* flecha) { flecha_ = flecha; }

private:
	Entity* flecha_;
	bool startAnimation;
};

