#include "PolvosAux.h"

#include <architecture/Entity.h>
#include <components/Render.h>
#include <components/Clickeable.h>
#include <components/DelayedCallback.h>
#include <architecture/Game.h>
#include <components/MoverTransform.h>
#include <sistemas/SpecialObjectsFactory.h>

PolvosAux::PolvosAux(ecs::Entity* paq) {
	paq_ = paq;
	clickedPolvos = false;
}

PolvosAux::~PolvosAux() {

}

void PolvosAux::initComponent() {
	ecs::Entity* polvosEspolv = ent_->getMngr()->addEntity(ecs::layer::STAMP);
	Transform* paqTr = paq_->getComponent<Transform>();
	Transform* polvTr = polvosEspolv->addComponent<Transform>(100, 100, 50, 50);
	polvTr->setParent(paqTr);
	polvosEspolv->addComponent<RenderImage>(&sdlutils().images().at("polvosEsparcidos"));

	Clickeable* clicPolvo = polvosEspolv->addComponent<Clickeable>("click");
	clicPolvo->addEvent([this,polvosEspolv] {
		if (!clickedPolvos)
		{
			clickedPolvos = true;
			SpecialObjectsFactory fact = SpecialObjectsFactory();
			fact.makeDeathTransition();
			polvosEspolv->addComponent<DelayedCallback>(4, []{
				gm().requestChangeScene(ecs::sc::MAIN_SCENE, ecs::sc::DEATH_SCENE);
				});
		}
	});
}

void PolvosAux::update() {

}