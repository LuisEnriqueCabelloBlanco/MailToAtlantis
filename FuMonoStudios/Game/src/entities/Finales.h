#pragma once
#include <architecture/GeneralData.h>
#include <architecture/Scene.h>

class Finales
{
	/// <summary>
	/// IMPORTANTE
	/// Para usar esta clase correctamente debes inicializar un final con el metodo inicializarFinal()
	/// Si deseas cambiar el final que has inicializado y que se muestra en el periodico,
	/// deberas usar el updateFinal()
	/// </summary>
public:

	Finales();
	~Finales() { deleteFinal(); };

	// Carga el primer final en forma de periodico en la escena que desees
	// Notese que si inicializas por segunda vez, el updateFinal estara funcionando para
	// la inicializacion de periodico mas nueva, es decir, para ese segundo periodico
	static void inicializarFinal(ecs::Scene escene, Personaje npc, Felicidad felicidad);

	// Carga otro final despues de haberlo inicializado reusando las texturas
	static void updateFinal(ecs::Scene escene, Personaje npc, Felicidad felicidad);

	// Borra el periodico si es que hay (Deberas inicializarlo otra vez)
	static void deleteFinal();

	// Devuelve el texto del final que desees
	static std::string getFinal(Personaje npc, Felicidad nivelFelicidad);
private:
	static std::unordered_map<Personaje, std::unordered_map<Felicidad,std::string>> endTexts_;

	static ecs::Entity* periodico_;
	static ecs::Entity* imagenNpc_;
};

