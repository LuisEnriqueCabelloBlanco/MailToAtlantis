#include "PackageChecker.h"
#include "../architecture/Component.h"
#include "../architecture/Entity.h"
#include "Transform.h"
#include "Gravity.h"
#include "MoverTransform.h"
#include "Paquete.h"
#include "SelfDestruct.h"
#include "../architecture/GeneralData.h"
#include <list>
#include <functional>
#include <components/ErrorNote.h>
#include <QATools/DataCollector.h>
#include "../sistemas/NPCeventSystem.h"

PackageChecker::PackageChecker(pq::Distrito dis, ecs::MainScene* sc) : 
	toDis_(dis), extraCond_(),mainSc_(sc), tutSc_(nullptr)
{

}

PackageChecker::PackageChecker(pq::Distrito dis, ecs::TutorialScene* sc) :
	toDis_(dis), extraCond_(), tutSc_(sc), mainSc_(nullptr)
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
	tri->addCallback(call, generalData().DropIn);
}

void PackageChecker::addCondition(Condition newCond)
{
	extraCond_.push_back(newCond);
}

bool PackageChecker::checkPackage(Paquete* package)
{	
	bool correctPack = package->correcto() && checkAdditionalConditions(package);		
	return  (correctPack && package->bienSellado()) || (!correctPack && toDis_ == pq::Erroneo);
}

void PackageChecker::checkEntity(ecs::Entity* ent)
{
	//comprobamos si es un paquete
	if (ent->getComponent<Paquete>() != nullptr) {
		ent->getComponent<DragAndDrop>()->disableInteraction();

		Vector2D entPos = ent->getComponent<Transform>()->getPos();
		ent->removeComponent<Gravity>();

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

		ent->addComponent<SelfDestruct>(1,[this](){
			if (mainSc_ != nullptr)
				mainSc_->createPaquete(generalData().getPaqueteLevel());
			else if (tutSc_ != nullptr)
				tutSc_->packageSent();
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

		generalData().npcEventSys->checkPaqueteSent(ent->getComponent<Paquete>(),toDis_);

#ifdef QA_TOOLS

		dataCollector().recordPacage(ent->getComponent<Paquete>(), correct);

#endif // QA_TOOLS
	}
	else
	{
		if (ent->getComponent<ErrorNote>() != nullptr) {
			auto mover = ent->getComponent<MoverTransform>();
			mover->setEasing(Easing::EaseOutCubic);
			mover->setFinalPos(ent->getComponent<Transform>()->getPos() + Vector2D(-600, 0));
			mover->setMoveTime(1);
			mover->enable();
			ent->addComponent<SelfDestruct>(1);
		}
	}
}

bool PackageChecker::checkAdditionalConditions(Paquete* package)
{
	bool aditional = true;
	for (Condition call : extraCond_) {
		if (!call(package)) {
			aditional = false;
		}
	}
	return aditional;
}



