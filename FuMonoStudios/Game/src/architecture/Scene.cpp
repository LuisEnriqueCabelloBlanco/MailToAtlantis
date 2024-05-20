#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <architecture/Scene.h>
#include <architecture/Entity.h>
#include <components/Transform.h>
#include <sistemas/ComonObjectsFactory.h>
#include <iostream>

using objListIt = std::array<std::vector<Entity*>, ecs::layer::maxLayerId>::iterator;

namespace ecs {
	Scene::Scene() :objs_() {
		factory_ = new ComonObjectsFactory(this);
	}
	Scene::~Scene() {
		clearScene();
		delete factory_;
#ifdef _DEBUG
		std::cout << "Se Destruyo correctamente la escena"<<std::endl;
#endif // _DEBUG
	}

	void Scene::init()
	{
	}
	void Scene::close()
	{
		clearScene();
		SoundEmiter::instance()->close();
	}
	void Scene::update() {
		for (int i = 0; i < objs_.size();i++ ) {
			for (int j = 0; j < objs_[i].size();j++) {
				auto e = objs_[i][j];
				if (e->isActive())
					e->update();

			}	
		}
	}
	void Scene::render() {
		for (int i = 0; i < objs_.size(); i++) {
			for (int j = 0; j < objs_[i].size(); j++) {
				auto e = objs_[i][j];
				if (e->isActive())
					e->render();
				}
			}
		}

	void Scene::deleteQueueEntities()
	{
		while (!del_.empty()) {
			delete* del_.front().second;
			objs_[del_.front().first].erase(del_.front().second);
			del_.pop();
		}
	}

	Entity* Scene::addEntity(ecs::layer::layerId lyId)
	{
		Entity* e = new Entity(this, lyId);
		e->setAlive(true);
		objs_[lyId].push_back(e);
		auto it = --objs_[lyId].end();
		e->addIterator(it);
		return e;
	}

	std::list<Entity*>::iterator Scene::addEntityToColisionList(Entity* e) {

		colisionEntities_.push_back(e);
		std::list<Entity*>::iterator it = colisionEntities_.end();
		return --it;
	}

	void Scene::removeEntity(std::vector<Entity*>::iterator it, ecs::layer::layerId lyId)
	{
		std::pair<ecs::layer::layerId, std::vector<Entity*>::iterator> e;
		e.first = lyId;
		e.second = it;
		del_.push(e);
	}

	void Scene::removeCollison(std::list<ecs::Entity*>::iterator it)
	{
		colisionEntities_.erase(it);
	}
	//Se pasa una entidad para comprobar si esta choca con el resto de entidades que tienen un trigger
	bool Scene::checkColisions(Entity* e) {

		bool ret = false;

		// cleon: iterador moderno. Es 2024.
		for (auto it = colisionEntities_.begin(); it != colisionEntities_.end(); ++it) {

			if ((*it) != e) {

				//Se guardan los rect ya que con lo que devuelve getRect() el SDL_HasIntersection falla

				SDL_Rect rect1 = e->getComponent<Transform>()->getRect();

				SDL_Rect rect2 = (*it)->getComponent<Transform>()->getRect();

				if (SDL_HasIntersection(&rect1, &rect2)) {
					auto trig = e->getComponent<Trigger>();
					if (trig != nullptr) {
						trig->touchEntity((*it));
						ret = true;
					}
				}

			}

		}

		return ret;

	}
	void Scene::refresh()
	{
		for (ecs::lyId_t gId = 0; gId < ecs::layer::maxLayerId; gId++) {
			auto& grpEnts = objs_[gId];
			grpEnts.erase(
				std::remove_if(grpEnts.begin(), grpEnts.end(),
					[gId](Entity* e) {
						if (e->isAlive()) {
							return false;
						}
						else {
							delete e;
							return true;
						}
					}),
				grpEnts.end());
		}
	}

	void Scene::clearScene()
	{
		for (auto& ly : objs_) {
			for (auto& e : ly) {
				delete e;
				e = nullptr;
			}
			ly.clear();
		}
	}

	void Scene::removeEntitiesByLayer(ecs::layer::layerId layer) {
		// Recorrer todas las entidades en la capa especificada
		for (auto it = objs_[layer].begin(); it != objs_[layer].end(); ++it) {
			// Eliminar la entidad
			delete* it;
		}
		// Limpiar el vector de entidades de la capa
		objs_[layer].clear();
	}

}
