#include "EndWorkScene.h"
#include "../architecture/GeneralData.h"
#include "../architecture/Entity.h"
#include <string>
#include "../components/Transform.h"
#include "../components/Render.h"
#include "../sistemas/ComonObjectsFactory.h"
#include "../components/Paquete.h"
#include "../components/Clickeable.h"
#include "../architecture/Game.h"
EndWorkScene::EndWorkScene():Scene() {

}

EndWorkScene::~EndWorkScene() {

}



void EndWorkScene::init() {
	//generalData().updateMoney();

	std::string msg = "Money: ";
	msg += std::to_string(generalData().getMoney());
	Vector2D pos(LOGICAL_RENDER_WIDTH/2, LOGICAL_RENDER_HEITH-400);
	Vector2D dist(0, -300);
	factory_->setLayer(ecs::layer::UI); 
	factory_->createLabel(pos, msg, 50);
	factory_->createLabel(pos + Vector2D(0, -300), "Alquiler: -" + std::to_string(generalData().getRent()), 50);
	factory_->createLabel(pos + Vector2D(0, -400), "Fails: " + std::to_string(generalData().getFails()), 50);
	factory_->createLabel(pos + Vector2D(0, -500), "Corrects: " + std::to_string(generalData().getCorrects()), 50);
	auto call = []() {gm().requestChangeScene(ecs::sc::END_WORK_SCENE, ecs::sc::EXPLORE_SCENE); };
	factory_->createTextuButton(pos + Vector2D(0, 70), "Return To menu", 50,call);
	//generalData().resetFailsCorrects();
	generalData().setDia(generalData().getDia() + 1);
  
  if (money >= 100) {
		std::string msgPass = "Felicidades, la de chambear de la sabes! ";
		factory_->createLabel(Vector2D(500, 800), msgPass, 50);
		auto call1 = []() {gm().requestChangeScene(ecs::sc::END_WORK_SCENE, ecs::sc::EXPLORE_SCENE); };
		factory_->createTextuButton(pos + Vector2D(0, 70), "Next Day", 50, call1);
		generalData().resetFailsCorrects();
		int currentDay = generalData().getDia();
		generalData().setDia(currentDay++);
	}
	else
	{
		std::string msgPass = "Veo que la de chambear no te la sabes, estas deportado. ";
		factory_->createLabel(Vector2D(300, 800), msgPass, 50);
		auto call2 = []() {gm().requestChangeScene(ecs::sc::END_WORK_SCENE, ecs::sc::MENU_SCENE); };
		factory_->createTextuButton(pos + Vector2D(0, 70), "Return To Menu", 50, call2);
		generalData().resetFailsCorrects();
	}

}