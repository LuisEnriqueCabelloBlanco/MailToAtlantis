#pragma once
#include <utils/checkML.h>
#include "ecs.h"
//#include "Entity.h"

class Manager;
namespace ecs {

	class Entity;
	/// <summary>
	/// Clase base para todos los componentes del juego
	/// </summary>
	class Component {
	public:
		Component() : ent_() {}
		virtual ~Component() {}
		/// <summary>
		/// Establece a que componente pertenece
		/// </summary>
		/// <param name="ent">entidad a la que se le asigna el componente</param>
		inline void setContext(Entity* ent) {
			ent_ = ent;
		}
		/// <summary>
		/// Metodo llamado para inicializar el componente en cuestion 
		/// (util para llamar a otros componentes)
		/// </summary>
		virtual void initComponent() {};
		/// <summary>
		/// se llama cada frame
		/// </summary>
		virtual void update() {};
		/// <summary>
		/// encargado de renderizar
		/// </summary>
		virtual void render() const {};
	protected:
		/// <summary>
		/// entidad a la que pertenece el componente
		/// </summary>
		Entity* ent_; 
	};
}

