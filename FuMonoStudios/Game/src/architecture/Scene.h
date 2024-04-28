#pragma once
#include <utils/checkML.h>
#include "../sdlutils/SDLUtils.h"
#include "ecs.h"
#include <vector>
#include <array>
#include <queue>
//#include <sistemas/ComonObjectsFactory.h>
//#include "Entity.h"
class ComonObjectsFactory;
namespace ecs {


	class Entity;
	/// <summary>
	/// Clase base de las escenas que se usan en el juego
	/// </summary>
	class Scene
	{
	public:
		Scene();
		virtual ~Scene();

		/// <summary>
		/// metodo llamado para pasar informacion y para generar las entidades
		/// </summary>
		virtual void init();

		/// <summary>
		/// metodo que se llama al descargar una escena
		/// </summary>
		virtual void close();
		//Vamos a querer tener un handle input o procesamos en el update?

		/// <summary>
		/// funcion llamada cada tic del juego
		/// se encarga de la logica basica
		/// </summary>
		virtual void update();
		/// <summary>
		/// funcion dedicada a pintar las entidades en pantalla
		/// </summary>
		virtual void render();
		/// <summary>
		///	Borrar las entidades en la lista de borrado
		/// </summary>
		void deleteQueueEntities();

		/// <summary>
		/// Aniade una entidad vacia a la escena
		/// </summary>
		/// <returns>Entidad vacia</returns>
		Entity* addEntity(ecs::layer::layerId lyId = ecs::layer::DEFAULT);
		/// <summary>
		/// Aniade la entidad pasada a la lista de colisiones y devueve un iterador a la lista
		/// </summary>
		/// <param name="e"></param>
		/// <returns></returns>
		std::list<Entity*>::iterator addEntityToColisionList(Entity* e);
		/// <summary>
		/// Elimina la entidad indicada por el iterador pasado que se debe ubicar en la capa indicada
		/// </summary>
		/// <param name="it">Iterador de la entidad</param>
		/// <param name="id">Identificador de la capa donde se va a borrar el objeto</param>
		void removeEntity(std::vector<Entity*>::iterator it, ecs::layer::layerId id);
		/// <summary>
		/// Elimina objeto de la lista de entidades con colision
		/// </summary>
		/// <param name="it"></param>
		void removeCollison(std::list<Entity*>::iterator it);
		/// <summary>
		/// Comprueba si alguna entidad colisiona con la entidad pasada
		/// </summary>
		/// <param name="myTrans"></param>
		/// <returns></returns>
		bool checkColisions(Entity* myTrans);
		/// <summary>
		/// Elimina las entidades marcadas como muertas
		/// </summary>
		void refresh();
		/// <summary>
		/// Elimina todas las entidades de una capa
		/// </summary>
		/// <param name="layer"></param>
		void removeEntitiesByLayer(ecs::layer::layerId layer);

		/// <summary>
		/// devuelve el vector que contiene los elementos de la capa indicada
		/// 
		/// (no se si se esta haciendo de la forma mas eficiente
		/// </summary>
		/// <param name="ly"></param>
		/// <returns></returns>
		std::vector<Entity*> getLayerEntitys(ecs::layer::layerId ly)const{
			return objs_[ly];
		}

		/// <summary>
		/// Elimina todas las entidades que estuvieran en la escena
		/// </summary>
		void clearScene();

		ComonObjectsFactory* getFactory() { return factory_; }
	protected:
		/// <summary>
			/// Vector de los objetos que pertenecen a la escena
			/// </summary>
		std::array<std::vector<Entity*>, ecs::layer::maxLayerId> objs_;
		/// <summary>
		/// Lista de las entidades que pueden colisionar
		/// </summary>
		std::list<Entity*> colisionEntities_;

		std::queue<std::pair<ecs::layer::layerId, std::vector<Entity*>::iterator>> del_;
		/// <summary>
		/// Factory que permite incluir nuevos objetos a la escena de forma segura
		/// si se usa para crear texturas esta se encarga de destruirlas mas adelante
		/// </summary>
		ComonObjectsFactory* factory_;
	};
}

