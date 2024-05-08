#pragma once
#include <architecture/Component.h>
#include "architecture/Game.h"

class BombAux : public ecs::Component
{
public:

	__CMP_DECL__(ecs::cmp::BOMBAUX)

	BombAux();

	~BombAux();

	void RedPressed();
	void BluePressed();
private:
	ecs::MainScene* scene;
	bool bluePressed;
};