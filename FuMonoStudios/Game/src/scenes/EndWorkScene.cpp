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

EndWorkScene::EndWorkScene():Scene() {
}

EndWorkScene::~EndWorkScene() {
}

void EndWorkScene::init() {

	// Creamos fondo, avanzamos dia y creamos offsets para el posicionamiento de UI
	factory_->createImage(Vector2D(), Vector2D(LOGICAL_RENDER_WIDTH, LOGICAL_RENDER_HEITH),
		&sdlutils().images().at("fondoFinalTrabajo"));

	generalData().setDay(generalData().getDay() + 1);

	pos_ = Vector2D(LOGICAL_RENDER_WIDTH / 2 + 200, 100);
	Vector2D dist(0, -300);
	std::string msg;
	factory_->setLayer(ecs::layer::UI);
	animFinish = false;

	// Dia actual
	msg = "Dia " + std::to_string(generalData().getDay());
	factory_->createLabel(Vector2D(1350, 50), msg, 80);

	// Corrects y fails
	msg = "Corrects: " + std::to_string(generalData().getCorrects());
	createTextAnim(msg);

	msg = "Fails: " + std::to_string(generalData().getFails());
	createTextAnim(msg);

	// Gastos alquiler e ingreos por trabajo
	msg = "Gastos de Alquiler: -" + std::to_string(generalData().getRent()) + "$";
	createTextAnim(msg);

	msg = "Nómina del día: " + std::to_string(generalData().calcularDineroGanado()) + "$";
	createTextAnim(msg);

	// Salto de posicion para colocar el resto de cosas más abajo
	pos_ = pos_ + Vector2D(0, 200);

	// Dinero total
	msg = "Cuenta Bancaria: " + std::to_string(generalData().getMoney()) + "$";
	createTextAnim(msg);

}

void EndWorkScene::createTextAnim(std::string msg)
{
	totalAnims_++;
	// Ajustamos pos
	pos_ = pos_ + Vector2D(0, offset_);
	// Creamos label y la añadimos a la lista de anims
	ecs::Entity* text = factory_->createLabel(pos_, msg, 50);
	text->setActive(false);
	anims_.push_back(text);
}

void EndWorkScene::update()
{
	ecs::Scene::update();
	
	// cada animCooldown_ y siempre que el numero de anims efectuadas no sea mayor a totalAnims_
	// estaremos reproduciendo anims hasta llegar a las totalAnims_ que tenemos
	if (currentAnim_ < totalAnims_) {
		if (timer_.currTime() > animCooldown_ * 1000) {
			// reproducimos la siguiente anim
			animTextos(anims_[currentAnim_]);
			if (currentAnim_ > 2) {
				animNumeros(anims_[currentAnim_]);
			}
			timer_.reset();
			currentAnim_++;
		}
	}
	else {
		if (!animFinish) {
			createButtons();
			animFinish = true;
		}
	}

}

void EndWorkScene::animTextos(ecs::Entity* texto) {
	sdlutils().soundEffects().at("GuiImpact").play();
	texto->setActive(true);
}
void EndWorkScene::animNumeros(ecs::Entity* number) {
	std::cout << "Anim numeros";
}

void EndWorkScene::createButtons()
{
	// Comprobamos si el usuario ha perdido o sigue con dinero
	int money = generalData().getMoney();

	if (money > 0) {
		// Boton nuevo dia
		auto call = []() {gm().requestChangeScene(ecs::sc::END_WORK_SCENE, ecs::sc::EXPLORE_SCENE); };
		factory_->createTextuButton(pos_ + Vector2D(0, offset_), "Nuevo dia", 50, call);
		// Sonido
		sdlutils().soundEffects().at("MoneyProfits").play();
		generalData().resetFailsCorrects();
		int currentDay = generalData().getDay();
	}
	else
	{
		// Boton nueva partida
		auto call = []() {gm().requestChangeScene(ecs::sc::END_WORK_SCENE, ecs::sc::MENU_SCENE); };
		factory_->createTextuButton(pos_ + Vector2D(0, offset_), "Nueva Partida", 50, call);
		// Sonido
		sdlutils().soundEffects().at("LoseMoney").play();
		// Texto
		factory_->createLabel(pos_ + Vector2D(0, offset_ * 2), "No has pagado ¡Deportado!", 50);
		generalData().resetFailsCorrects();
		generalData().setDay(1);
		generalData().resetMoney();
	}
}


