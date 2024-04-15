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
#include "../components/MoverTransform.h"
#include "../components/DelayedCallback.h"

EndWorkScene::EndWorkScene():Scene() {

}

EndWorkScene::~EndWorkScene() {

}

void EndWorkScene::init() {

	// Creamos fondo, avanzamos dia y creamos offsets para el posicionamiento de UI
	factory_->createImage(Vector2D(), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH),
		&sdlutils().images().at("fondoFinalTrabajo"));

	generalData().setDia(generalData().getDia() + 1);

	Vector2D pos(LOGICAL_RENDER_WIDTH / 2 + 200, LOGICAL_RENDER_HEITH - 400);
	Vector2D dist(0, -300);
	factory_->setLayer(ecs::layer::UI);

	// Corrects y fails
	ecs::Entity* fails = factory_->createLabel(pos + Vector2D(0, -400), "Fails: " + std::to_string(generalData().getFails()), 50);
	ecs::Entity* corrects = factory_->createLabel(pos + Vector2D(0, -500), "Corrects: " + std::to_string(generalData().getCorrects()), 50);

	// Gastos alquiler e ingreos por trabajo
	factory_->createLabel(pos + Vector2D(0, -200), "Gastos de Alquiler: -" + std::to_string(generalData().getRent()) + "$", 50);
	std::string msg = "Nómina del día: ";
	msg += std::to_string(generalData().calcularDineroGanado()) + "$";
	ecs::Entity* nomina = factory_->createLabel(pos + Vector2D(0, -300), msg, 50);

	// Dinero total
	msg = "Dinero Total: ";
	msg += std::to_string(generalData().getMoney()) + "$";
	ecs::Entity* total = factory_->createLabel(pos + Vector2D(0, 100), msg, 50);

	// Dia actual
	msg = "Dia ";
	msg += std::to_string(generalData().getDia());
	factory_->createLabel(Vector2D(1350, 50), msg, 80);
	
	// Comprobamos si el usuario ha perdido o sigue con dinero
	int money = generalData().getMoney();

	if (money >= 0) {
		// Boton nuevo dia
		auto call = []() {gm().requestChangeScene(ecs::sc::END_WORK_SCENE, ecs::sc::EXPLORE_SCENE); };
		factory_->createTextuButton(pos + Vector2D(0, 170), "Nuevo dia", 50, call);
		// Sonido
		sdlutils().soundEffects().at("MoneyProfits").play();
		auto call1 = []() {gm().requestChangeScene(ecs::sc::END_WORK_SCENE, ecs::sc::EXPLORE_SCENE); };
		generalData().resetFailsCorrects();

	}
	else
	{
		// Boton nueva partida
		auto call = []() {gm().requestChangeScene(ecs::sc::END_WORK_SCENE, ecs::sc::MAIN_SCENE); };
		factory_->createTextuButton(pos + Vector2D(0, 70), "Nueva Partida", 50, call);
		// Sonido
		sdlutils().soundEffects().at("LoseMoney").play();
		// Texto
		std::string msgPass = "No has pagado ¡Deportado!";
		factory_->createLabel(Vector2D(pos.getX(), 800), msgPass, 50);
		// Ajustes
		auto call2 = []() {gm().requestChangeScene(ecs::sc::END_WORK_SCENE, ecs::sc::MENU_SCENE); };
		generalData().resetFailsCorrects();
		generalData().setDia(1);
		generalData().resetMoney();
	}
	animTextos(corrects, fails);
	//animNumeros(total);
}

void EndWorkScene::update()
{
	ecs::Scene::update();

}


void EndWorkScene::animTextos(ecs::Entity* corrects, ecs::Entity* fails) {

	// Anim corrects
	corrects->addComponent<DelayedCallback>(1.0f, [this, corrects]() {
		sdlutils().soundEffects().at("GuiImpact").play();
		corrects->setActive(true);
	});

	// Anim fails
	fails->addComponent<DelayedCallback>(1.0f, [this, fails]() {
		sdlutils().soundEffects().at("GuiImpact").play();
		fails->setActive(true);
	});
}
void EndWorkScene::animNumeros(ecs::Entity* nomina, ecs::Entity* totalMoney) {

}