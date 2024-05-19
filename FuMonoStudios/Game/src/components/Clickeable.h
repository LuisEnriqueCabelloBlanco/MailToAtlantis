#pragma once

#include <architecture/Component.h>
#include <functional>
#include <list>
#include <string>

class Transform;

using CallbackClickeable = std::function<void(void)>;

class Clickeable : public ecs::Component {

public:
	__CMP_DECL__(ecs::cmp::CLICKEABLE)
	Clickeable(std::string soundClick);

	~Clickeable();

	void initComponent() override;

	void update() override;

	/// <summary>
	/// Aniade a la lista de eventos el evento indicado
	/// </summary>
	/// <param name="event"></param>
	void addEvent(CallbackClickeable event );

	/// <summary>
	/// Borra tododos los eventos de la lista de eventos
	/// </summary>
	void deleteEvents();

	/// <summary>
	/// Permite acitivar o desactivar si el objeto puede ser clickado o no
	/// </summary>
	/// <param name="onoff"></param>
	void toggleClick(bool onoff) { canClick_ = onoff; }
private:

	Transform* mTr_;
	/// <summary>
	/// Eventos que seran llamados al clicar sobre el objeto
	/// </summary>
	std::list<CallbackClickeable> eventsWhenClick_;
	/// <summary>
	/// Etiqueta del sonido que suena al clicar sobre el
	/// </summary>
	std::string soundWhenClicked_;
	/// <summary>
	/// Indica si se va a detectar el click sobre el objeto
	/// </summary>
	bool canClick_;

};
