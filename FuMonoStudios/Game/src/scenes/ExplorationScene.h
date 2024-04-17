﻿#pragma once
#include "../architecture/Scene.h"
#include "../components/DialogManager.h"
#include "../sistemas/ComonObjectsFactory.h"
#include <list>

constexpr float SCALE_NPCS = 0.25;
class NPCeventSystem;

namespace ecs {

	/// <summary>
	/// Struct que guarda la informaci�n de cada lugar, tiene el fondo a renderizar, un booleano para saber si se 
	/// puedenavegar a �l, un mapa con las direcciones que conectan a �l (a las que no tiene por qu� poder navegarse),
	/// y una lista de punteros a las entidades propias del lugar (perosonajes y flechas de movimiento) 
	/// Tiene m�todos para a�adir direcciones al lugar (necesita un string y una instancia de lugar), un booleano
	/// que indica si cierto lugar (indicado con el string del mapa) es navegable, un getPlaceFromDirection 
	/// que devuelve un puntero apuntando la posici�n de memoria de un lugar del mapa de direcciones (se usa para 
	/// moverte por el mapa) y un getTexture (se usa para renderizar el background en el mapa).
	/// No hay destructora porque no se genera nueva memoria din�mica.
	/// </summary>
	/// 
	struct Lugar {
	public:
		//constructoras
		Lugar() : ents_() {};

		Lugar(Texture* t, bool n) : ents_(), backGround_(t), navegable_(n) /*character(c)*/ {};

		/// <summary>
		/// Metodo para anadir direcciones al mapa del lugar
		/// </summary>
		/// <param name="placeDir"></param>
		/// <param name="place"></param>
		void addDirections(std::string placeDir, Lugar* place);

		/// <summary>
		/// Metodo para comprobar la navegabilidad a ciero lugar
		/// </summary>
		/// <param name="placeDir"></param>
		/// <returns></returns>
		bool navigate(std::string placeDir);

		/// <summary>
		/// Metodo para obtener la direccion de memoria de un lugar del mapa de direcciones (se usa para navegar)
		/// </summary>
		Lugar* getPlaceFromDirection(std::string placeDir);
		/// <summary>
		/// Metodo que devuelve la textura del fondo para poder renderizarla
		/// </summary>
		/// <returns></returns>
		Texture* getTexture() const { return backGround_; };

		void changeActivationObjects(bool state);

		/// <summary>
		/// Crea los objetos del lugar actual al que te acabas de mover.
		/// USAR DESPUES DE HABER NAVEGADO
		/// </summary>
		void addObjects(ecs::Entity* e);

		/// <summary>
		/// método para comprobar la propia navegabilidad de un lugar
		/// </summary>
		bool isNavegable() const;

		/// <summary>
		/// Método para setear la navegabilidad a true o false en función de value, vale true si no introduces valor
		/// </summary>
		/// <param name="value"></param>
		void setNavegability(bool value = true);



	private:
		//Puntero a la textura del fondo
		Texture* backGround_;

		//Bool que permite la navegacion
		bool navegable_;

		//Mapa con las direcciones adyacentes al lugar (a las que no tiene por qu� poderse navegar)
		std::unordered_map<std::string, Lugar*> directions_;

		// Las entidades del lugar se almacenan como las de la escena, en vectores de objetos organizados en layouts
		std::vector<ecs::Entity*> ents_;

	};

	
	//CLASE
    class ExplorationScene :
        public Scene

    {
    public:

        ExplorationScene();
        virtual ~ExplorationScene();
        void init() override;
        virtual void close() override {}
        void render();
		void update();

		/// <summary>
		/// Metodo para navegar a cierto lugar
		/// </summary>
		void navigate(std::string placeDir);

		/// <summary>
		/// Metodo para renderizar el backGround
		/// </summary>
		void renderBackGround() const;

		// cierra la conversacion
		void closeConversation();
    private:
		
		/// <summary>
		/// Metodo para inicializar los lugares del mapa
		/// </summary>
		void initPlacesDefaultMap();

		/// <summary>
		/// Metodo para incializar las direcciones adyacentes a cada lugar del mapa
		/// </summary>
		void initDirectionsDefaultMap();

		/// <summary>
		/// Mata (setAlive(false) los objetos del lugar para que se borren de la escena y los borra del vector
		/// del lugar.
		/// USAR ANTES DE NAVEGAR SI ES QUE SE PUEDE NAVEGAR
		/// </summary>
		void killObjects();

		/// <summary>
		/// Crea los objetos del lugar actual al que te acabas de mover.
		/// USAR DESPUES DE HABER NAVEGADO
		/// </summary>
		void createObjects(int place);

		/// <summary>
		/// Metodo factoria para las flechas de navegacion
		/// </summary>
		ecs::Entity* createNavegationsArrows(Vector2D pos, std::string place, float scale, int flip);

		ecs::Entity* createWorkButton(Vector2D pos, Vector2D scale);

		/// <summary>
		/// Metodo factoria para characters
		/// </summary>

		/// <summary>
		/// Método para setar la navegabilidad de placeDir lugar, valor por defecto = true
		/// </summary>
		/// <param name="placeDir"></param>
		/// <param name="value"></param>
		void setNavegabilityOfPlace(int place, bool value = true);

		/// <summary>
		/// Método para actualizar la navegabilidad según el día
		/// </summary>
		void updateNavegavility();
		ecs::Entity* createCharacter(Vector2D pos, const std::string& character, float scale);
        
		//VARIABLES

		//unordered_map, Lugar*
		std::unordered_map<std::string, Lugar> lugares;

		//Puntero al lugar actual
		Lugar* actualPlace_;	

		//rect para renderizar el BackGround
		SDL_Rect rect_;

		DialogManager dialogMngr_;

		int placeToGo;
	
		// entidades del dialogo
		ecs::Entity* boxBackground;
		ecs::Entity* textDialogue;

		// flag para saber si podemos entablar dialogo
		bool canStartConversation;

		ecs::Entity* boton_Trabajo;
    };
}


