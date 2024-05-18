#pragma once
#include <architecture/Component.h>

class Clickeable;
class ImageRender;
class MoverTransform;

class PolvosAux : public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::POLVOSAUX)

	PolvosAux(ecs::Entity* paq);

	~PolvosAux();

	void initComponent() override;

	void update() override;

private:
	ecs::Entity* paq_;
	bool clickedPolvos;
};

