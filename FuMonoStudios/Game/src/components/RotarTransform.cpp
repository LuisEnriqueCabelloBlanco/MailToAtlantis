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

	assert(myTransform != nullptr);

	myGrades_ = 0;
	desiredGrades_ = 0;
	animGrades_ = 3;
}

void RotarTransform::update()
{
	if (myGrades_ != desiredGrades_) {
		if (myGrades_ < desiredGrades_) {
			myTransform->setRotation(myTransform->getRotation() + animGrades_);
			myGrades_ += animGrades_;
			if (myGrades_ > desiredGrades_)
				myGrades_ = desiredGrades_;
		}
			
		else {
			myTransform->setRotation(myTransform->getRotation() - animGrades_);
			myGrades_ -= animGrades_;
			if (myGrades_ < desiredGrades_)
				myGrades_ = desiredGrades_;
		}
	}
}
