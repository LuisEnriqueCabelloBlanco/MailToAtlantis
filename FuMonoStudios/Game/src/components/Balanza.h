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
	/// <summary>
	/// inicia la animacion de pesado de la balanza
	/// </summary>
	/// <param name="paquete"></param>
	/// <param name="balanzaB"></param>
	/// <param name="flechaRotComp"></param>
	void initAnimations(ecs::Entity* paquete, ecs::Entity* balanzaB, RotarTransform* flechaRotComp);
	/// <summary>
	/// finaliza la animacion de pesado de la balanza
	/// </summary>
	/// <param name="paquete"></param>
	/// <param name="flechaRotComp"></param>
	void finishAnimatios(ecs::Entity* paquete, RotarTransform* flechaRotComp);

	/// <summary>
	/// Inicia la animacion de pesado de la balanza digital
	/// </summary>
	/// <param name="paquete"></param>
	/// <param name="balanzaB"></param>
	void initAnimationsDigital(ecs::Entity* paquete, ecs::Entity* balanzaB);
	/// <summary>
	/// Finaliza la animaicon de pesado de la balanza digital
	/// </summary>
	/// <param name="paquete"></param>
	void finishAnimatiosDigital(ecs::Entity* paquete);
	int getPaquetePeso() {return paquetePeso_;};

private:
	Transform* myTransform_;
	bool startAnimation;
	int paquetePeso_;
};

