#pragma once

#include "../architecture/Component.h"

class Transform;

class RotarTransform: public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::ROTARTRANSFORM)
	RotarTransform();
	virtual ~RotarTransform();

	virtual void initComponent();
	virtual void update() override;

	void setDesiredGrades(float desiredGrades) { desiredGrades_ = desiredGrades; }

private:
	// mis grados
	float myGrades_;
	// grados que quiero
	float desiredGrades_;
	//Cantidad de grados a subir o bajar
	float animGrades_;
	Transform* myTransform;
};