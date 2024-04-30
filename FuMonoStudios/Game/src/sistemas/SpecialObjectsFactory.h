#pragma once
#include <architecture/Scene.h>
#include "../architecture/Entity.h"

class RenderImage;
class DragAndDrop;
// Similar al CommonObjectsFactory pero para objetos especiales de la historia
// o de eventos especiales
class SpecialObjectsFactory
{
public:
	enum NombreObjeto {
		ListaNombresDia5Vagabundo
	};

	SpecialObjectsFactory(ecs::Scene* scene);
	~SpecialObjectsFactory();


	ecs::Entity* makeObject(NombreObjeto obj);
private:
	ecs::Entity* makeListaVagabundo();

	ecs::Scene* scene;

	std::vector<Texture*> createdTextures;
};

