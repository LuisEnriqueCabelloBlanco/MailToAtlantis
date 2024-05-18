#pragma once

#include <architecture/Component.h>
#include <components/Render.h>
#include <components/Paquete.h>
#include <architecture/Entity.h>

#include <functional>
#include <list>
#include <SDL.h>


class Transform;

class Trigger;

class MultipleTextures;

//posibles puntos en los que se puede detectar el raton
enum pointRoute {

	LeftUp,
	MiddleUp,
	RightUp,
	LeftMid,
	MiddleMid,
	RightMid,
	LeftDown,
	MiddleDown,
	RightDown

};

class Wrap : public ecs::Component {

public:
	__CMP_DECL__(ecs::cmp::WRAP);

	Wrap(float spaceAux, int repTimesAux, const std::list<int>& routeAux, int routeIndex);

	Wrap(float spaceAux, int repTimesAux);

	Wrap(float spaceAux);

	~Wrap();

	void initComponent() override;

	void update() override;

	//devuelve si el paquete esta envuelto
	bool isWrapped() { return wrapped; }

private:

	//Reinicia la ruta para cuando se deba repetir un patron x veces
	void restartRoute();

	//Comprueba si ha chocado con el punto especificado por última vez
	void checkPointTouch(int point);



	struct Line {
		SDL_Point start;
		SDL_Point end;
	};

	Transform* tr_ = nullptr;

	Trigger* tri_ = nullptr;

	DragAndDrop* dad_ = nullptr;

	RenderImage* mul_ = nullptr;

	RenderImage* renderImage_ = nullptr;

	std::vector<Line> linesDrawn; // Almacena las líneas dibujadas

	//Ruta con los distintos puntos por los que debe pasar el ratón
	std::list<int> route;

	//ultimo punto por el que se a pasado de la ruta
	std::list<int>::iterator lastPoint;

	std::list<int>::iterator beforePoint;

	//radio en el cual se considera que se a pasado por el punto
	float space = 5;

	//repeticiones del patron que hay que hacer para considerarlo envuelto
	int repTimes = 0;

	//si esta envuelto
	bool wrapped = false;

	//Cuantos puntos de ruta lleva hechos el jugador
	int routePointsDone = 0;

	//por que fase de los sprites de envalado va
	int wrapFase = 0;

	//Numero total de puntos de ruta que hay que hacer para envalarlo del todo
	int totalPointsRoute = 4;

	//debug para futuras implementaciones
	bool debug = true;

	int routeSelectedID;

	// Textura de punto rojo
	Texture* puntoRojoTex = nullptr;

	Callback funcion_;

	Paquete* paqComp_ = nullptr;

	Texture* puntoAzulTex = nullptr;

	bool puntoCreado = false;

};
