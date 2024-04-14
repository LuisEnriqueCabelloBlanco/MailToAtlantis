#pragma once

#include "../architecture/Component.h"

class Transform;

class RotarTransform: public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::ROTARTRANSFORM)

	virtual void initComponent();
	virtual void update() override;

private:
	// mis grados
	float myGrades_;
	// grados que quiero
	float desiredGrades_;
	//Cantidad de grados a subir o bajar
	float animGrades_;
	// 
	Transform* myTransform;
};