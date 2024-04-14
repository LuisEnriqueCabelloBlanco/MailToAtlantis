#include "RotarTransform.h"
#include "Transform.h"

void RotarTransform::update()
{
	if (myGrades_ != desiredGrades_) {
		Vector2D pos = myTransform->getPos();
		if (myGrades_ < desiredGrades_)
			myTransform->setRotation(myTransform->getRotation() + animGrades_);
		else
			myTransform->setRotation(myTransform->getRotation() - animGrades_);
	}
}
