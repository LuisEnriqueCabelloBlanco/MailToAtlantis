#pragma once
#include <architecture/GeneralData.h>
#include <architecture/Scene.h>

class Final
{
	/// <summary>
	// Carga el final en la escena que necesites. Para ello solo crea una instancia de final.
	/// </summary>
public:

	// Carga el final en la escena que necesites
	Final(ecs::Scene* escene, Personaje npc, Felicidad felicidad);
	~Final();

	// Carga el siguiente final en la escena donde inicializaste Final
	void loadFinal(Personaje npc, Felicidad felicidad);

	// Devuelve el texto del final que desees
	std::string getFinal(Personaje npc, Felicidad nivelFelicidad);
private:
	// Inicializa endTexts_
	void inicializarFinal(ecs::Scene* escene, Personaje npc, Felicidad felicidad);

	// endTexts_ es un mapa con todos nuestros finales
	static std::unordered_map<Personaje, std::unordered_map<Felicidad,std::string>> endTexts_;

	ecs::Scene* escene_; // Donde se crea el final, lo usamos en loadFinal para saber en que escena estamos
	ecs::Entity* periodico_;
	ecs::Entity* imagenNpc_;
	ecs::Entity* texto_;
};

