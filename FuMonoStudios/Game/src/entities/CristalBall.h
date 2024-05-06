#pragma once
#include <components/Paquete.h>
#include <components/Render.h>

#pragma once
class CristalBall : public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::CRISTALBALL)
	CristalBall(RenderImage*);
	virtual ~CristalBall();		
	void check(Paquete*, bool);

private:	
	int showing;
	RenderImage* myRender;
};

