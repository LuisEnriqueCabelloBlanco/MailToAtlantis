#include "PackageChecker.h"
#include <components/SelfDestruct.h>
#include <functional>
#include <components/ErrorNote.h>
#include <QATools/DataCollector.h>
#include <sistemas/SoundEmiter.h>
#include <sistemas/NPCeventSystem.h>

PackageChecker::PackageChecker(pq::Distrito dis, ecs::MainScene* sc, PipeManager* mngr) : 
	toDis_(dis),mainSc_(sc), tutSc_(nullptr), mManager_(mngr)
{

}

PackageChecker::PackageChecker(pq::Distrito dis, ecs::TutorialScene* sc, PipeManager* mngr) :
	toDis_(dis), tutSc_(sc), mainSc_(nullptr), mManager_(mngr)
{

}

PackageChecker::~PackageChecker()
{
}

void PackageChecker::initComponent()
{

	std::function<void(ecs::Entity*)> call = [this](ecs::Entity* ent) {checkEntity(ent); };
	Trigger* tri = ent_->getComponent<Trigger>();
	assert(tri != nullptr);
	tri->addCallback(call, gD().DropIn);
}

/*void PackageChecker::addCondition(Condition newCond)
{
	extraCond_.push_back(newCond);
}*/

bool PackageChecker::checkPackage(Paquete* package)
{	
	/*bool correctPack = package->correcto() && checkAdditionalConditions(package);
	return  (correctPack && package->bienSellado()) || (!correctPack && toDis_ == pq::Erroneo);*/
	return mManager_->checkPackage(package, toDis_);
}

void PackageChecker::checkEntity(ecs::Entity* ent)
{
	//comprobamos si es un paquete
	if (ent->getComponent<Paquete>() != nullptr) {
		SoundEmiter::instance()->playSound("tubo");
		//Desactivamos las cosas necesarias para hacer una animación
		ent->getComponent<DragAndDrop>()->disableInteraction();
		ent->removeComponent<Gravity>();

		Vector2D entPos = ent->getComponent<Transform>()->getPos();

		//animacion de salida del paquete dependiaendo de que sea
		auto mover = ent->getComponent<MoverTransform>();
		if (toDis_ != Erroneo) {
			mover->setEasing(Easing::EaseOutCubic);
			mover->setFinalPos(entPos+ Vector2D(0, -600));
			mover->setMoveTime(1.7f);
		}
		else {
			mover->setEasing(Easing::EaseOutCubic);
			mover->setFinalPos(entPos+ Vector2D(-600, 0));
			mover->setMoveTime(1);
		}
		mover->enable();

		
		ent->addComponent<SelfDestruct>(1, [this, ent]() {
			if (ent->hasComponent(ecs::cmp::POLVOSAUX)) {
				gm().requestChangeScene(ecs::sc::MAIN_SCENE, ecs::sc::END_SCENE);
			}
			else
			{
				if (mainSc_ != nullptr)
					mainSc_->createPaquete(gD().getPaqueteLevel());
				else if (tutSc_ != nullptr)
					tutSc_->packageSent();
			}
		});
		bool correct = checkPackage(ent->getComponent<Paquete>());
		if (correct) {

			GeneralData::instance()->correctPackage();
		}
		else {			
			GeneralData::instance()->wrongPackage();
			if (mainSc_ != nullptr)
				mainSc_->createErrorMessage(ent->getComponent<Paquete>(), toDis_ == Erroneo,toDis_ != ent->getComponent<Paquete>()->getDistrito());
			else if (tutSc_ != nullptr)
				tutSc_->createErrorMessage(ent->getComponent<Paquete>(), toDis_ == Erroneo, toDis_ != ent->getComponent<Paquete>()->getDistrito());
				
		}

		gD().npcEventSys->checkPaqueteSent(ent->getComponent<Paquete>(),toDis_);

#ifdef QA_TOOLS

		dataCollector().recordPacage(ent->getComponent<Paquete>(), correct);

#endif // QA_TOOLS
	}
	else
	{
		if (ent->getComponent<ErrorNote>() != nullptr && toDis_ == Erroneo) {
			auto mover = ent->getComponent<MoverTransform>();
			mover->setEasing(Easing::EaseOutCubic);
			mover->setFinalPos(ent->getComponent<Transform>()->getPos() + Vector2D(-600, 0));
			mover->setMoveTime(1);
			mover->enable();
			ent->addComponent<SelfDestruct>(1);
		}
	}
}

/*
bool PackageChecker::checkAdditionalConditions(Paquete* package)
{
	bool aditional = true;
	for (Condition call : extraCond_) {
		if (!call(package)) {
			aditional = false;
		}
	}
	return aditional;
}*/



