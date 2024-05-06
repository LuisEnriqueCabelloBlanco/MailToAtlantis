#pragma once

class Game;

class Time
{
public:
	static float getDeltaTime() { return deltaTime_; }
private:
	friend class Game;
	static float deltaTime_;
};

