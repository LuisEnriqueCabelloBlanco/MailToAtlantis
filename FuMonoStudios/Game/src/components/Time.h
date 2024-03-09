#pragma once
#include "../sdlutils/SDLUtils.h"
#include "../architecture/Game.h"
class Time
{
public:
	static float getDeltaTime() { return deltaTime_; }
private:
	friend class Game;
	static float deltaTime_;
};

