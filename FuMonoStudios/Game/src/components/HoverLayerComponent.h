#pragma once
#include <list>
#include <functional>

#include <architecture/Component.h>
class Transform;
/*Compoente que detecta el hover de entidades de una capa determinada sobre esta entidad*/
class HoverLayerComponent :
    public ecs::Component
{
public:
    __CMP_DECL__(ecs::cmp::HOVERLAYER)

    HoverLayerComponent(ecs::layer::layerId detectLayer);
    ~HoverLayerComponent();

	virtual void update() override;

	void initComponent() override;
	/// <summary>
	/// Aniade una funcion que se llamara al meter una entidad en la hitbox
	/// </summary>
	/// <param name="hoverCall"></param>
	void addInCall(std::function<void()> hoverCall);
	/// <summary>
	/// Aniade una funcion que se llamara al sacar una entidad de la hitbox
	/// </summary>
	/// <param name="hoverCall"></param>
	void addOutCall(std::function<void()> hoverCall);

private:
	/// <summary>
	/// Llama a todas las InCalls
	/// </summary>
	void activateInCalls();
	/// <summary>
	/// Llama a todas las OutCalls
	/// </summary>
	void activateOutCalls();

	ecs::layer::layerId detectionLayer_;
	/// <summary>
	/// estado en el frame anterior
	/// </summary>
	bool inside_;
	Transform* mTr_;
	std::list<std::function<void()>> inCalls_;
	std::list<std::function<void()>> outCalls_;
};

