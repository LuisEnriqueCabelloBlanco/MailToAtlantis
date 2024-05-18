#pragma once
#include <architecture/Component.h>
#include <architecture/Entity.h>
#include <components/Transform.h>

class Depth : public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::DEPTH)

	Depth();
	~Depth();
	void initComponent() override;

	/// <summary>
	/// Acutaliza la profundidad del objeto en funcion de su altura
	/// </summary>
	void updateDepth();
	/// <summary>
	/// Actualiza la posicion de los objetos hijos tras ser escalado el objeto
	/// </summary>
	void updateChildPos();
private:
	float depth_;
	Transform* tr_;
};