#include "RotarTransform.h"
#include "Transform.h"
#include "../architecture/Entity.h"

RotarTransform::RotarTransform(): myTransform(nullptr)
{
}

RotarTransform::~RotarTransform()
{
}

void RotarTransform::initComponent()
{
	myTransform = ent_->getComponent<Transform>();

	assert(myTransform == nullptr);
}

void RotarTransform::update()
{
	if (myGrades_ != desiredGrades_) {
		if (myGrades_ < desiredGrades_)
			myTransform->setRotation(myTransform->getRotation() + animGrades_);
		else
			myTransform->setRotation(myTransform->getRotation() - animGrades_);
	}
}
