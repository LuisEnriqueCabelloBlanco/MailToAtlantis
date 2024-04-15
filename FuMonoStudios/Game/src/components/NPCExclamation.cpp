#include "NPCExclamation.h"
#include "../architecture/Entity.h"
#include "../components/Render.h"
#include "../components/MoverTransform.h"
NPCExclamation::NPCExclamation() {			
 }
NPCExclamation::~NPCExclamation() {
	
}
void NPCExclamation::innit (int x, int y) {	
	Texture* textura = &sdlutils ().images ().at ("exclamacion");
	tr_ = ent_->addComponent<Transform> (x, y, textura->width (), textura->height ());	
	tr_->setScale (0.7f);
	ent_->addComponent<RenderImage> (textura);

	goUp ();	
}
void NPCExclamation::goUp() {
	//ent_->removeComponent<MoverTransform>();
	ent_->addComponent<MoverTransform>(tr_->getPos() - Vector2D(0, 30),
		2, Easing::EaseOutBack, [this]() {
			goDown();
		})->enable();
}
void NPCExclamation::goDown() {
	//ent_->removeComponent<MoverTransform>();
	ent_->addComponent<MoverTransform>(tr_->getPos() + Vector2D(0, 30),
		2, Easing::EaseOutBack, [this]() {
			goUp();
		})->enable();
}