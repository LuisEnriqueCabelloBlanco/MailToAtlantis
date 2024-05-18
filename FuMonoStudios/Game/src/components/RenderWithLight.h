
#pragma once
#include <architecture/Component.h>

class Transform;
class Texture;
class Clickeable;
class RenderWithLight: public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::IMAGEWITHLIGHT)
	RenderWithLight();
	RenderWithLight(Texture*);
	RenderWithLight(Texture*, ecs::layer::layerId);
	~RenderWithLight();
	/// <summary>
	/// Activa el objeto luz
	/// </summary>
	void lightOn();
	/// <summary>
	/// Desactiva el objeto luz
	/// </summary>
	void lightOff();

	void initComponent() override;
private:
	/// <summary>
	/// Textura del recuadro
	/// </summary>
	Texture* lTexture_;
	/// <summary>
	/// Transform sobre el que se va a renderizar la imagen
	/// </summary>
	Transform* mTr_;

	ecs::Entity* lightEnt_;

	ecs::layer::layerId layer_;
};

