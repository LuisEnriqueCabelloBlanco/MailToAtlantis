#pragma once
#include <list>
#include <functional>

#include <architecture/Component.h>
class Transform;
/*Componente que detecta cuando el cursor pasa por encima de la entidad*/
class HoverSensorComponent:public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::HOVERSENSOR)

	HoverSensorComponent();
	~HoverSensorComponent(); 


	virtual void update() override;

	void initComponent() override;
	/// <summary>
	/// Aniade funciones que se llamaran al pasar el cursor por encima de la entidad
	/// </summary>
	/// <param name="hoverCall"></param>
	void addInCall(std::function<void()> hoverCall);
	/// <summary>
	/// Aniade funcines que se llamaran al sacar el cursar de la entidad
	/// </summary>
	/// <param name="hoverCall"></param>
	void addOutCall(std::function<void()> hoverCall);
	void addDestoryCall(std::function<void()> destroyCall);

private:
	/// <summary>
	/// Llama a las InCalls
	/// </summary>
	void activateInCalls();
	/// <summary>
	/// Llama a las OutCalls
	/// </summary>
	void activateOutCalls();

	bool inside_;
	Transform* mTr_;
	std::list<std::function<void()>> inCalls_;
	std::list<std::function<void()>> outCalls_;
	std::list<std::function<void()>> destroyCalls_;
};

