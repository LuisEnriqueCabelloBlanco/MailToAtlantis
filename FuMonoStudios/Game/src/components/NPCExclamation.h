#pragma once
#include <architecture/Component.h>
#include <components/Transform.h>

class NPCExclamation : public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::NPC_EXCLAMATION);
	NPCExclamation();
	~NPCExclamation();
	void innit(int, int);
private:
	void goUp();
	void goDown();
	Transform* tr_;
};