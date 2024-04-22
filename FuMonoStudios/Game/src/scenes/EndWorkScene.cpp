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

	factory_->createImage(Vector2D(), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH),
		&sdlutils().images().at("fondoFinalTrabajo"));

	std::string msg = "Money: ";
	msg += std::to_string(generalData().getMoney());
	Vector2D pos(LOGICAL_RENDER_WIDTH/2 + 200, LOGICAL_RENDER_HEITH-400);
	Vector2D dist(0, -300);
	factory_->setLayer(ecs::layer::UI); 
	factory_->createLabel(pos, msg, 50);
	factory_->createLabel(pos + Vector2D(0, -300), "Alquiler: -" + std::to_string(generalData().getRent()), 50);
	factory_->createLabel(pos + Vector2D(0, -400), "Fallos: " + std::to_string(generalData().getFails()), 50);
	factory_->createLabel(pos + Vector2D(0, -500), "Correctos: " + std::to_string(generalData().getCorrects()), 50);
	auto call = []() {gm().requestChangeScene(ecs::sc::END_WORK_SCENE, ecs::sc::EXPLORE_SCENE); };
	factory_->createTextuButton(pos + Vector2D(0, 70), "Nuevo dia", 50,call);
	//generalData().resetFailsCorrects();
	generalData().setDay(generalData().getDay() + 1);
  
	int money = generalData().getMoney();

	if (money >= 0) {
		std::string msgPass = "Pasaste el dia ";
		factory_->createLabel(Vector2D(pos.getX(), 800), msgPass, 50);
		auto call1 = []() {gm().requestChangeScene(ecs::sc::END_WORK_SCENE, ecs::sc::EXPLORE_SCENE); };
		generalData().resetFailsCorrects();
		int currentDay = generalData().getDay();
	}
	else
	{
		std::string msgPass = "No has pagado, deportado ";
		factory_->createLabel(Vector2D(pos.getX(), 800), msgPass, 50);
		auto call2 = []() {gm().requestChangeScene(ecs::sc::END_WORK_SCENE, ecs::sc::MENU_SCENE); };
		generalData().resetFailsCorrects();
		generalData().setDay(1);
	}

}