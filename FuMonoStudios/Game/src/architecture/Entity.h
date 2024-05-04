#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "ecs.h"
#include <algorithm>
#include <array>
#include "Component.h"
#include "Scene.h"
#include "../components/Trigger.h"
#include "../components/Transform.h"
#include "../components/DragAndDrop.h"
//class Manager;

namespace ecs {
	class Entity
	{
	public:

		Entity(Scene* scene, ecs::layer::layerId ly) : scene_(scene), cmps_(), currCmps_(), alive_(), myLayer(ly), mySpecifyLayer(ly), enable_(true),active_(true) {
			currCmps_.reserve(cmp::maxComponentId);
		};

		virtual ~Entity() {
			for (auto c : currCmps_) {
				delete c;
			}
		};
		/// <summary>
		/// Inica si la entidad esta viva
		/// </summary>
		/// <returns></returns>
		inline bool isAlive() const { return alive_; };
		/// <summary>
		/// Indica si la entidad esta activa
		/// </summary>
		/// <returns></returns>
		inline bool isActive() const { return active_; };
		/// <summary>
		/// AVISO: funcion en desuso si se quiere usar revisar el codigo fuente
		/// </summary>
		/// <returns></returns>
		inline bool isEnable() const { return enable_; };
		/// <summary>
		/// Determina si en el siguente frame se destruira la entidad
		/// </summary>
		/// <param name="alive"></param>
		inline void setAlive(bool alive) { alive_ = alive; };
		/// <summary>
		/// Determina si una entidad va a estar activa
		/// </summary>
		/// <param name="active"></param>
		inline void setActive(bool active) { 

			Transform* myTR = getComponent<Transform>();

			if (myTR != nullptr) {
				myTR->setActiveChildren(active);
			}

			active_ = active; 
			if (active) {
				myLayer = mySpecifyLayer;
			}
			else {
				myLayer = ecs::layer::INACTIVE;
			}
		};
		/// <summary>
		/// AVISO: funcionalidad en desuso
		/// </summary>
		/// <param name="enable"></param>
		inline void setEnable(bool enable) { enable_ =enable; };

		/// <summary>
		/// Accesor a la escena permite a los componentes 
		/// realizar operaciones como meter nuevas entidades a la escena
		/// TODO: cambiar el nombre
		/// </summary>
		/// <returns></returns>
		inline Scene* getMngr() const { return scene_; };

		/// <summary> 
		///	Modelo predeterminado del add entity al cual se mete pasandole cualquier valor excepto los especificos
		/// </summary>
		template<typename T, typename ...Ts>
		inline T* addComponent(Ts&&... args) {

			T* c = addComponent_aux<T>(args...);

			return c;

		}

		/// <summary>
		/// Sobreescritura del add component especificando funcionalidad extra necesaria para el Trigger
		/// </summary>
		/// <returns></returns>
		template<>
		inline Trigger* addComponent<Trigger>() {

			Trigger* t = addComponent_aux<Trigger>(scene_->addEntityToColisionList(this));

			return t;

		}

		/// <summary>
		/// Elimina el componente de Entity marcado por cId
		/// </summary>
		/// <typeparam name="T"></typeparam>
		template<typename T>
		inline void removeComponent() {
			if (cmps_[cmpId<T>] != nullptr) {
				auto iter = std::find(currCmps_.begin(),
					currCmps_.end(),
					cmps_[cmpId<T>]);
				currCmps_.erase(iter);
				delete cmps_[cmpId<T>];
				cmps_[cmpId<T>] = nullptr;
			}
		}

		/// <summary>
		/// Devuelve un puntero al componente de Entity marcado por cId
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<typename T>
		inline T* getComponent() {

			// the component id
			constexpr cmpId_t cId = cmpId<T>;
			static_assert(cId < ecs::cmp::maxComponentId);

			return static_cast<T*>(cmps_[cId]);
		}
		/// <summary>
		/// Devuelve al capa a la que pertenece la entidad
		/// </summary>
		/// <returns></returns>
		inline ecs::layer::layerId getLayer() {
			return myLayer;
		}

		inline void addIterator(std::vector<Entity*>::iterator it) {
			mIt_ = it;
		}

		inline std::vector<Entity*>::iterator getIterator() {
			return mIt_;
		}

		/// <summary>
		/// Comprueba si Entity tiene el componente marcado por cId
		/// </summary>
		/// <param name="cId"></param>
		/// <returns></returns>
		inline bool hasComponent(ecs::cmpId_t cId) {
			return cmps_[cId] != nullptr;
		}

		/// <summary>
		/// Llama al update de cada componente activo en la entidad
		/// </summary>
		inline void update() {
			auto n = currCmps_.size();
			for (auto i = 0u; i < n; i++) {
				currCmps_[i]->update();
			}
		}
		/// <summary>
		/// Llama al render de cada componente activo en la entidad
		/// </summary>
		inline void render() {
			auto n = currCmps_.size();
			for (auto i = 0u; i < n; i++) {
				currCmps_[i]->render();
			}
		}

	private:
		/// <summary>
		/// determina si la entidad va a destruirse si esta a false
		/// </summary>
		bool alive_;
		/// <summary>
		/// determina si un objeto se tiene que renderizar
		/// </summary>
		bool active_; //esto es mentira, no solo para el render tambien el update
		/// <summary>
		/// determina si un objeto se tiene que actualizar
		/// </summary>
		bool enable_; //???? esto es otra mentira, no hace na (os acabo de mentir, si que hace, se carga toda la ejecucion del programa, no usar a no ser que lo arregles)
		/// <summary>
		/// puntero a la escena a la que pertenece al entidad
		/// </summary>
		Scene* scene_;

		std::vector<Entity*>::iterator mIt_;
		/// <summary>
		/// Capa a la que pertenece la entidad
		/// </summary>
		ecs::layer::layerId myLayer = ecs::layer::DEFAULT;
		//La layer que se especifico al principio para la entidad, esta no se cambia nunca y es una referencia para poder volver a ella
		ecs::layer::layerId mySpecifyLayer = ecs::layer::DEFAULT;
		/// <summary>
		/// vector de componentes acitvos en la entidad
		/// </summary>
		std::vector<Component*> currCmps_;
		/// <summary>
		/// vector de componentes en la entidad
		/// </summary>
		std::array<Component*, cmp::maxComponentId> cmps_;


		/// <summary>
		/// A�ade un componente a Entity Tener cuidado si se añaden
		/// varias veces un componente -> puede llevar a error
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <typeparam name="...Ts"></typeparam>
		/// <param name="cId">Identificador del componente</param>
		/// <param name="...args">Argumentos de la constructora del componente a a�adir</param>
		/// <returns>Puntero al componente creado</returns>
		template<typename T, typename ...Ts>
		inline T* addComponent_aux(Ts&&... args) {
			T* c = new T(std::forward<Ts>(args)...);
			removeComponent<T>();

			currCmps_.push_back(c);
			cmps_[cmpId<T>] = c;

			c->setContext(this);
			c->initComponent();

			return c;
		}
	};

}

