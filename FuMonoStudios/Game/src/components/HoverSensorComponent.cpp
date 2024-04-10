#include "HoverSensorComponent.h"
#include <sdlutils/InputHandler.h>
#include <SDL.h>
#include <components/Transform.h>
HoverSensorComponent::HoverSensorComponent():mTr_(nullptr), hoverCalls_()
{

}

HoverSensorComponent::HoverSensorComponent(std::function<void()> hoverCall):mTr_(nullptr),hoverCalls_()
{
	addCall(hoverCall);
}

HoverSensorComponent::~HoverSensorComponent()
{
}

void HoverSensorComponent::update()
{
	auto& ihdlr = ih();
	SDL_Point point{ ihdlr.getMousePos().first, ihdlr.getMousePos().second };
	if (SDL_PointInRect(&point,&mTr_->getRect())) {

	}
}

void HoverSensorComponent::addCall(std::function<void()> hoverCall)
{
	hoverCalls_.push_back(hoverCall);
}
