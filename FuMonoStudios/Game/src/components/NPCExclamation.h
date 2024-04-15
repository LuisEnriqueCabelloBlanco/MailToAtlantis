#pragma once
#include "../architecture/Component.h"
#include "Transform.h"

class NPCExclamation : public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::NPC_EXCLAMATION);
	NPCExclamation(int, int);
	~NPCExclamation();
private:
	void goUp();
	void goDown();
	Transform* tr_;
};