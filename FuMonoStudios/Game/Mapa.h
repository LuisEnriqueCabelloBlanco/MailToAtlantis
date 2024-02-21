#pragma once
#include "src/sdlutils/Texture.h"
#include "Character.h"
#include <unordered_map>


struct Lugar {
	Texture* backGround;
	bool navegable;
	//Character* character;
	std::unordered_map<std::string, Lugar> directions;
	Lugar() {};
	Lugar(Texture* t, bool n /*Character* c*/) : backGround(t), navegable(n) /*character(c)*/ {};
	void addDirections(std::string placeDir, Lugar place);
	bool navigate(std::string placeDir);
};

class Mapa
{

private:
	Lugar* actualPlace;

	//Luego har� un vector y un enum
	Lugar demeter;
	Lugar hefesto;
	Lugar hestia;
	Lugar artemisa;
	Lugar hermes;
	Lugar apolo;
	Lugar poseidon;

	void initPlacesDefaultMap();
	void initDirectionsDefaultMap();

public:
	Mapa();
	~Mapa();
	void navigate(std::string placeDir);
};

