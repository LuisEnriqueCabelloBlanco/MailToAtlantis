#pragma once
#include <architecture/Component.h>
#include <functional>
#include <string>

class Transform;
class Gravity;
class Trigger;

using SimpleCallback = std::function<void()>; // por si quieres q haga algo al soltar

// Este comp permite mover una entity con transform y al soltarlo sobre una
// entity con Trigger, se activan los callbacks de la entidad soltada.
// Es importante NO AÑADIR UN TRIGGER si se ha añadido este componente antes
// Puedes añadir un callback para la entidad que arrastras, para ello asignalo
// en la constructora.
// Debes meter un bool diciendo si quieres que se haga callbacks de todas las entidades
// que están tocando o solo la que esta mas cercana
class DragAndDrop : public ecs::Component {

public:
	__CMP_DECL__(ecs::cmp::DRAGANDDROP)
	DragAndDrop();
	DragAndDrop(std::string sound);
	DragAndDrop(bool usingClosestEnt, std::string sound);
	DragAndDrop(bool usingClosestEnt, bool usingOwnCallback, std::string sound);
	DragAndDrop(bool usingClosestEnt, SimpleCallback func, std::string sound);

	~DragAndDrop();
	static bool enableDrag;

	void initComponent() override;

	void update() override;

	bool isDragging() {
		return dragging_;
	}

	void disableInteraction() { canInteract = false; }
	void activateInteraction() { canInteract = true; }
private:
	bool canInteract = true;

	Transform* tr_;
	Gravity* grav_;
	Trigger* tri_;

	bool dragging_;
	float differenceX_;
	float differenceY_;

	// para el escalado al arrastrar objetos
	float porcentajeStart;

	bool usingOnlyClosestEnt_ = false;
	bool usingCallback_ = false;
	bool usingOwnCallback_ = false;
	SimpleCallback func_;
	std::pair<int,int> latestPoint_;

	std::string draggingSound_;
};
