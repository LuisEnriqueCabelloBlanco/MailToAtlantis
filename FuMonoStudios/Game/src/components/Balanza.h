#pragma once
#include <architecture/Component.h>
class RotarTransform;
class Transform;

class Balanza: public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::BALANZA)
		Balanza();

	~Balanza();

	void initComponent() override;

	void initAnimations(ecs::Entity* paquete, ecs::Entity* balanzaB, RotarTransform* flechaRotComp);
	void finishAnimatios(ecs::Entity* paquete, RotarTransform* flechaRotComp);

	//Balanza Digital
	void initAnimationsDigital(ecs::Entity* paquete, ecs::Entity* balanzaB);
	void finishAnimatiosDigital(ecs::Entity* paquete);
	int getPaquetePeso() {return paquetePeso_;};

private:
	Transform* myTransform_;
	bool startAnimation;
	int paquetePeso_;
};

