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

	//Se añade un evento a la lista. Este se activara cada vez que la entidad sea clickeada
	void addEvent(CallbackClickeable event );

	//borra todos los eventos actuales
	void deleteEvents();

	//Te permite activar o desactivar si el objeto puede ser clickeado o no
	void toggleClick(bool onoff) { canClick_ = onoff; }
private:

	Transform* mTr_;

	std::list<CallbackClickeable> eventsWhenClick_;

	std::string soundWhenClicked_;

	bool canClick_;

};
