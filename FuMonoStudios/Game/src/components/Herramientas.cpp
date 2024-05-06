#include "Herramientas.h"
#include <components/Transform.h>
#include <components/MoverTransform.h>

Herramientas::Herramientas() {
	multicolorStamp = false;
}

Herramientas::~Herramientas() {

}

void Herramientas::setFunctionality(TipoHerramienta tipo) {

	switch (tipo) 
	{
	case SelloCalleA:
		funcion_ = [this](ecs::Entity* paq) {
			Paquete* paqComp = paq->getComponent<Paquete>();
			paqComp->sellarCalle(pq::C1, ent_->getComponent<Transform>(), false);
			ent_->addComponent<MoverTransform>(Vector2D(230, 800), 0.5, EaseOutBack);
			ent_->getComponent<MoverTransform>()->enable();
			};
		break;
	case SelloCalleB:
		funcion_ = [this](ecs::Entity* paq) {
			Paquete* paqComp = paq->getComponent<Paquete>();
			paqComp->sellarCalle(pq::C2, ent_->getComponent<Transform>(), false);
			ent_->addComponent<MoverTransform>(Vector2D(230, 800), 0.5, EaseOutBack);
			ent_->getComponent<MoverTransform>()->enable();
			};
		break;
	case SelloCalleC:
		funcion_ = [this](ecs::Entity* paq) {
			Paquete* paqComp = paq->getComponent<Paquete>();
			paqComp->sellarCalle(pq::C3, ent_->getComponent<Transform>(), false);
			ent_->addComponent<MoverTransform>(Vector2D(230, 800), 0.5, EaseOutBack);
			ent_->getComponent<MoverTransform>()->enable();
			};
		break;
	case SelloMultiColor:
		multicolorStamp = true;
		funcion_ = [this](ecs::Entity* paq) {
			Paquete* paqComp = paq->getComponent<Paquete>();			
			paqComp->sellarCalle(paq->getComponent<Paquete>()->getCalle(), ent_->getComponent<Transform>(), true);			
			ent_->addComponent<MoverTransform>(Vector2D(230, 800), 0.5, EaseOutBack);
			ent_->getComponent<MoverTransform>()->enable();
			};
		break;
	
	}
#ifdef _DEBUG
	std::cout << "El tipo de herramienta es: " << tipo << std::endl;
#endif // _DEBUG

}
void Herramientas::interact(ecs::Entity* paquete) {
	funcion_(paquete);
}