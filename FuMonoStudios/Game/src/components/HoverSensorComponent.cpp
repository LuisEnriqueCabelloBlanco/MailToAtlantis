#include "HoverSensorComponent.h"
#include <sdlutils/InputHandler.h>
#include <SDL.h>
#include <components/Transform.h>
#include <architecture/Entity.h>
HoverSensorComponent::HoverSensorComponent():mTr_(nullptr), inCalls_(),inside_(false)
{

}

HoverSensorComponent::~HoverSensorComponent()
{
	for (auto func : destroyCalls_) {
		func();
	}
}

void HoverSensorComponent::update()
{
	auto& ihdlr = ih();
	SDL_Point point{ ihdlr.getMousePos().first, ihdlr.getMousePos().second };
	Vector2D pos = mTr_->getPos();

	SDL_Rect rect = build_sdlrect(pos, mTr_->getWidth(), mTr_->getHeigth());
	bool actualState = SDL_PointInRect(&point, &rect);
	if (actualState && !inside_) {
		activateInCalls();
	}

	if (!actualState && inside_){
		activateOutCalls();
	}

	inside_ = actualState;
}

void HoverSensorComponent::initComponent()
{
	mTr_ = ent_->getComponent<Transform>();
}

void HoverSensorComponent::addInCall(std::function<void()> hoverCall)
{
	inCalls_.push_back(hoverCall);
}

void HoverSensorComponent::addOutCall(std::function<void()> hoverCall)
{
	outCalls_.push_back(hoverCall);
}

void HoverSensorComponent::addDestoryCall(std::function<void()> destroyCall)
{
	destroyCalls_.push_back(destroyCall);
}

void HoverSensorComponent::activateInCalls()
{
	for (auto func : inCalls_) {
		func();
	}
}

void HoverSensorComponent::activateOutCalls()
{
	for (auto func : outCalls_) {
		func();
	}
}
