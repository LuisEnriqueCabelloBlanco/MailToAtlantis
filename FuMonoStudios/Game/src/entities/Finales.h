#pragma once
#include <architecture/GeneralData.h>
#include <architecture/Scene.h>
//#include <tuple>

class Finales
{
public:

	Finales();
	~Finales() {};

	// Carga el final en forma de periodico en la escena que desees.
	void loadFinal(Personaje npc, Felicidad nivelFelicidad, Scene scene);

	// Devuelve el texto del final que desees
	std::string getFinal(Personaje npc, Felicidad nivelFelicidad);
private:
	std::unordered_map<std::pair<Personaje, Felicidad> , std::string> endTexts_;
};

