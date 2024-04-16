#include "RenderWithLight.h"
#include "../architecture/Component.h"
#include "Transform.h"
#include "../architecture/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "Clickeable.h"
#include <sistemas/ComonObjectsFactory.h>

RenderWithLight::RenderWithLight() : mTr_(nullptr),lightEnt_(nullptr){ lTexture_ = &sdlutils().images().at("luz"); }

RenderWithLight::RenderWithLight(Texture* img) : mTr_(nullptr),lightEnt_(nullptr){ lTexture_ = img; }

RenderWithLight::~RenderWithLight() {
}

void RenderWithLight::lightOn()
{
	lightEnt_->setActive(true);
}

void RenderWithLight::lightOff()
{
	lightEnt_->setActive(false);
}

void RenderWithLight::initComponent() {
	mTr_ = ent_->getComponent<Transform>();
	auto fact = ent_->getMngr()->getFactory();
	auto ly = fact->getLayer();
	fact->setLayer(ecs::layer::LIGHT);
	lTexture_->modColor(200, 200, 200);
	lightEnt_ = fact->createImage(Vector2D(0,0),
		Vector2D(mTr_->getWidth(),mTr_->getHeigth()),lTexture_);
	lightEnt_->setActive(false);
	auto tr = lightEnt_->getComponent<Transform>();
	tr->setParent(mTr_);
	fact->setLayer(ly);
	
	assert(mTr_ != nullptr);
}
