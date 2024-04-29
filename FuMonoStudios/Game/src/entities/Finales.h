#pragma once
#include <architecture/GeneralData.h>
#include <architecture/Scene.h>

class Finales
{
public:

	Finales();
	~Finales() {};

	// Carga el final en forma de periodico en la escena que desees.
	void loadFinal(ecs::Scene escene, Personaje npc, Felicidad felicidad);

	// Devuelve el texto del final que desees
	std::string getFinal(Personaje npc, Felicidad nivelFelicidad);
private:
	std::unordered_map<std::pair<Personaje, Felicidad> , std::string> endTexts_;
};

