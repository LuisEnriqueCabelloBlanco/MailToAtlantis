#include "Balanza.h"
#include "../architecture/Entity.h"
#include "Paquete.h"
#include "RotarTransform.h"
#include "MoverTransform.h"

Balanza::Balanza()
{
}

Balanza::~Balanza()
{
}

void Balanza::initComponent()
{
}

void Balanza::initAnimations(ecs::Entity* paquete, RotarTransform* flechaRotComp)
{
	int cantidadPeso = paquete->getComponent<Paquete>()->getCantidadPeso();
	std::cout << cantidadPeso;
	flechaRotComp->setDesiredGrades(cantidadPeso);
}

void Balanza::finishAnimatios(RotarTransform* flechaRotComp)
{
	flechaRotComp->setDesiredGrades(0);
}

