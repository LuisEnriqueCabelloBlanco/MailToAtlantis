#include "Balanza.h"
#include "../architecture/Entity.h"
#include "Paquete.h"
#include "Gravity.h"
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

	//Obtenemos Tr de balanza
	myTransform_ = ent_->getComponent<Transform>();

	assert(myTransform_ != nullptr);
}

void Balanza::initAnimations(ecs::Entity* paquete, ecs::Entity* balanzaB, RotarTransform* flechaRotComp)
{
	if (paquete->hasComponent(ecs::cmp::PAQUETE)) {

		//Obtenemos cantidad peso
		int cantidadPeso = paquete->getComponent<Paquete>()->getCantidadPeso();

		//Animamos flecha
			flechaRotComp->setDesiredGrades(cantidadPeso);

		//Animamos paquete
			MoverTransform* paqueteMovComp = paquete->getComponent<MoverTransform>();
			Transform* paqTr = paquete->getComponent<Transform>();
			Gravity* paqGr = paquete->getComponent<Gravity>();
			paqueteMovComp->setEasing(Easing::EaseOutCubic);
			paqueteMovComp->setFinalPos(Vector2D(myTransform_->getPos().getX() + myTransform_->getWidth() / 2 - paqTr->getWidth() / 2
				, myTransform_->getPos().getY() + 70));
			paqueteMovComp->setMoveTime(1.0f);
			paqueteMovComp->enable();
			paqGr->setActive(false);

		//Animamos balanza
			/*balanzaB->setLayer(ecs::layer::BALANZAB);*/
			MoverTransform* balanzaMovComp = ent_->getComponent<MoverTransform>();
			balanzaMovComp->setEasing(Easing::EaseOutCubic);
			balanzaMovComp->setFinalPos(Vector2D(myTransform_->getRelPos().getX(), myTransform_->getRelPos().getY() + 20));
			balanzaMovComp->setMoveTime(1.0f);
			balanzaMovComp->enable();
	}
}

void Balanza::finishAnimatios(ecs::Entity* paquete, RotarTransform* flechaRotComp)
{
	if (paquete->hasComponent(ecs::cmp::PAQUETE)) {
		flechaRotComp->setDesiredGrades(0);

		//Animamos balanza
				/*balanzaB->setLayer(ecs::layer::BALANZAB);*/
		MoverTransform* balanzaMovComp = ent_->getComponent<MoverTransform>();
		Gravity* paqGr = paquete->getComponent<Gravity>();
		balanzaMovComp->setEasing(Easing::EaseOutCubic);
		balanzaMovComp->setFinalPos(Vector2D(myTransform_->getRelPos().getX(), myTransform_->getRelPos().getY() - 20));
		balanzaMovComp->setMoveTime(1.0f);
		balanzaMovComp->enable();
		paqGr->setActive(true);
	}
}

void Balanza::initAnimationsDigital(ecs::Entity* paquete, ecs::Entity* balanzaB) {
	if (paquete->hasComponent(ecs::cmp::PAQUETE)) {

		//Obtenemos cantidad peso
		 paquetePeso_ = paquete->getComponent<Paquete>()->getCantidadPeso();

		//Animamos paquete
		MoverTransform* paqueteMovComp = paquete->getComponent<MoverTransform>();
		Transform* paqTr = paquete->getComponent<Transform>();
		Gravity* paqGr = paquete->getComponent<Gravity>();
		paqueteMovComp->setEasing(Easing::EaseOutCubic);
		paqueteMovComp->setFinalPos(Vector2D(myTransform_->getPos().getX() + myTransform_->getWidth() / 2 - paqTr->getWidth() / 2
			, myTransform_->getPos().getY() - 160));
		paqueteMovComp->setMoveTime(1.0f);
		paqueteMovComp->enable();
		paqGr->setActive(false);

		//Animamos balanza
			/*balanzaB->setLayer(ecs::layer::BALANZAB);*/
		MoverTransform* balanzaMovComp = ent_->getComponent<MoverTransform>();
		balanzaMovComp->setEasing(Easing::EaseOutCubic);
		balanzaMovComp->setFinalPos(Vector2D(myTransform_->getRelPos().getX(), myTransform_->getRelPos().getY() + 7));
		balanzaMovComp->setMoveTime(1.0f);
		balanzaMovComp->enable();
		
	}
}


void Balanza::finishAnimatiosDigital(ecs::Entity* paquete) {

	if (paquete->hasComponent(ecs::cmp::PAQUETE)) {

		//Animamos balanza
				/*balanzaB->setLayer(ecs::layer::BALANZAB);*/
		MoverTransform* balanzaMovComp = ent_->getComponent<MoverTransform>();
		Gravity* paqGr = paquete->getComponent<Gravity>();
		balanzaMovComp->setEasing(Easing::EaseOutCubic);
		balanzaMovComp->setFinalPos(Vector2D(myTransform_->getRelPos().getX(), myTransform_->getRelPos().getY() - 1));
		balanzaMovComp->setMoveTime(1.0f);
		balanzaMovComp->enable();
		paqGr->setActive(true);
	}

}

