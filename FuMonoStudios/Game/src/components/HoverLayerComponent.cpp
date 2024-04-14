#include "HoverLayerComponent.h"
#include <sdlutils/InputHandler.h>
#include <SDL.h>
#include <components/Transform.h>
#include <architecture/Entity.h>
HoverLayerComponent::HoverLayerComponent(ecs::layer::layerId detectLayer):
	mTr_(nullptr), inCalls_(), inside_(false),detectionLayer_(detectLayer)
{
}

HoverLayerComponent::~HoverLayerComponent()
{
	
}

void HoverLayerComponent::update()
{
	Vector2D mPos = mTr_->getPos();
	SDL_Rect mRect = build_sdlrect(mPos, mTr_->getWidth(), mTr_->getHeigth());
	auto layer = ent_->getMngr()->getLayerEntitys(detectionLayer_);

	for (auto& ent : layer) {
		Transform* eTr = ent->getComponent<Transform>();
		Vector2D ePos = eTr->getPos();
		SDL_Rect eRect = build_sdlrect(ePos, eTr->getWidth(), eTr->getHeigth());
		bool intersect = SDL_HasIntersection(&mRect, &eRect);
		if ( intersect && !inside_) {
			activateInCalls();
		} 
		if (!intersect && inside_) {
			activateOutCalls();
		}
		inside_ = intersect;
	}
}

void HoverLayerComponent::initComponent()
{
	mTr_ = ent_->getComponent<Transform>();
}

void HoverLayerComponent::addInCall(std::function<void()> hoverCall)
{
	inCalls_.push_back(hoverCall);
}

void HoverLayerComponent::addOutCall(std::function<void()> hoverCall)
{
	outCalls_.push_back(hoverCall);
}

void HoverLayerComponent::activateInCalls()
{
	for (auto func : inCalls_) {
		func();
	}
}

void HoverLayerComponent::activateOutCalls()
{
	for (auto func : outCalls_) {
		func();
	}
}
