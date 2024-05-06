#pragma once
#include <list>
#include <functional>

#include <architecture/Component.h>
class Transform;
class HoverLayerComponent :
    public ecs::Component
{
public:
    __CMP_DECL__(ecs::cmp::HOVERLAYER)

    HoverLayerComponent(ecs::layer::layerId detectLayer);
    ~HoverLayerComponent();

	virtual void update() override;

	void initComponent() override;
	void addInCall(std::function<void()> hoverCall);
	void addOutCall(std::function<void()> hoverCall);

private:
	void activateInCalls();
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

