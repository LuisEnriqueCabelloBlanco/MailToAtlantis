#include "Herramientas.h"
#include "Transform.h"
#include "MoverTransform.h"

Herramientas::Herramientas() {

}

Herramientas::~Herramientas() {

}

void Herramientas::setFunctionality(TipoHerramienta tipo) {

	switch (tipo) 
	{
	case SelloCalleA:
		funcion_ = [this](ecs::Entity* paq) {
			Paquete* paqComp = paq->getComponent<Paquete>();
			paqComp->sellarCalle(pq::C1, ent_->getComponent<Transform>());
			ent_->addComponent<MoverTransform>(Vector2D(230, 800), 0.5, EaseOutBack);
			ent_->getComponent<MoverTransform>()->enable();
			};
		break;
	case SelloCalleB:
		funcion_ = [this](ecs::Entity* paq) {
			Paquete* paqComp = paq->getComponent<Paquete>();
			paqComp->sellarCalle(pq::C2, ent_->getComponent<Transform>());
			ent_->addComponent<MoverTransform>(Vector2D(230, 800), 0.5, EaseOutBack);
			ent_->getComponent<MoverTransform>()->enable();
			};
		break;
	case SelloCalleC:
		funcion_ = [this](ecs::Entity* paq) {
			Paquete* paqComp = paq->getComponent<Paquete>();
			paqComp->sellarCalle(pq::C3, ent_->getComponent<Transform>());
			ent_->addComponent<MoverTransform>(Vector2D(230, 800), 0.5, EaseOutBack);
			ent_->getComponent<MoverTransform>()->enable();
			};
		break;
	}
	std::cout << "El tipo de herramienta es: " << tipo << std::endl;
}
void Herramientas::interact(ecs::Entity* paquete) {
	funcion_(paquete);
}