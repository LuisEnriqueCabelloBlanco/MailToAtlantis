#include "NPCExclamation.h"
#include "../architecture/Entity.h"
#include "../components/Render.h"
#include "../components/MoverTransform.h"
NPCExclamation::NPCExclamation(int x, int y) {	
	Texture* textura = &sdlutils().images().at("exclamacion");	
	std::cout << "A";
	tr_ = ent_->addComponent<Transform>(x, y, textura->width(), textura->height());
	std::cout << "B";
	ent_->addComponent<RenderImage>(textura);	
	goUp();
 }
NPCExclamation::~NPCExclamation() {
	
}
void NPCExclamation::goUp() {
	//ent_->removeComponent<MoverTransform>();
	ent_->addComponent<MoverTransform>(tr_->getPos() - Vector2D(0, 50),
		3, Easing::EaseOutBack, [this]() {
			goDown();
		})->enable();
}
void NPCExclamation::goDown() {
	//ent_->removeComponent<MoverTransform>();
	ent_->addComponent<MoverTransform>(tr_->getPos() + Vector2D(0, 50),
		3, Easing::EaseOutBack, [this]() {
			goUp();
		})->enable();
}