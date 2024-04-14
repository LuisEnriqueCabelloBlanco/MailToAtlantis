#pragma once

#include "../architecture/Component.h"
class RotarTransform: public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::ROTARTRANSFORM)

	virtual void initComponent() {};
	virtual void update() {};
private:
	//mis
	float mGrades_;
};