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
	

	SpecialObjectsFactory(ecs::Scene* scene);
	~SpecialObjectsFactory();

	// llamar al empezar el dia y elegira que objetos especiales poner
	void setupDayObjects();
private:

	void makePapelAgujeros();

	void makeListaVagabundo();

	ecs::Scene* scene;

	std::vector<Texture*> createdTextures;
};

