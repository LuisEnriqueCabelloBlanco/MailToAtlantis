#pragma once
#include <scenes/MainScene.h>
#include <components/Paquete.h>
#include <scenes/TutorialScene.h>
#include <functional>

using Condition = std::function<bool(Paquete*)>;
class NPCeventSystem;

class PackageChecker : public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::CHECKER)
	PackageChecker(pq::Distrito, ecs::MainScene* sc, PipeManager* mngr);
	PackageChecker(pq::Distrito, ecs::TutorialScene* sc, PipeManager* mngr);
	~PackageChecker();

	virtual void initComponent();
	//void addCondition(Condition);

	/// <summary>
	/// Comprueba si un paquete es correcto
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	bool checkPackage(Paquete*);
	/// <summary>
	/// Realiza el anialisis de validez del envio
	/// </summary>
	/// <param name="ent"></param>
	void checkEntity(ecs::Entity* ent);
private:
	//bool checkAdditionalConditions(Paquete*);

	pq::Distrito toDis_;
	//std::list<Condition> extraCond_;
	ecs::MainScene* mainSc_;
	ecs::TutorialScene* tutSc_;
	PipeManager* mManager_;
};

