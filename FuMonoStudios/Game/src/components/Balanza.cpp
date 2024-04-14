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
	if (paquete->hasComponent(ecs::cmp::PAQUETE)) {

		//Obtenemos cantidad peso
		int cantidadPeso = paquete->getComponent<Paquete>()->getCantidadPeso();
		std::cout << cantidadPeso;

		//Obtenemos Tr de balanza
		Transform* balanzaTr = ent_->getComponent<Transform>();

		//Animamos flecha
			flechaRotComp->setDesiredGrades(cantidadPeso);

		//Animamos paquete
			MoverTransform* paqueteMovComp = paquete->getComponent<MoverTransform>();
			Transform* paqTr = paquete->getComponent<Transform>();
			paqueteMovComp->setEasing(Easing::EaseOutCubic);
			paqueteMovComp->setFinalPos(Vector2D(balanzaTr->getPos().getX() + balanzaTr->getWidth() / 2 - paqTr->getWidth() / 2
				, balanzaTr->getPos().getY() + 100));
			paqueteMovComp->setMoveTime(1.0f);
			paqueteMovComp->enable();

		//Animamos balanza
	}
}

void Balanza::finishAnimatios(RotarTransform* flechaRotComp)
{
	flechaRotComp->setDesiredGrades(0);
}

