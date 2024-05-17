#pragma once
#include <architecture/Scene.h>
#include <architecture/Entity.h>

class RenderImage;
class DragAndDrop;
// Similar al CommonObjectsFactory pero para objetos especiales de la historia
// o de eventos especiales
class SpecialObjectsFactory
{
public:

	SpecialObjectsFactory();

	~SpecialObjectsFactory();

	ecs::Entity* makeBomba();

	ecs::Entity* makePolvos();

	// dura 4 seg
	void makeDeathTransition();

	// llamar al empezar el dia y elegira que objetos especiales poner
	void setupDayObjects();
private:

	void makePapelAgujeros();

	void makeListaVagabundo();

	void makeListaSecretario(bool first);

	std::vector<Texture*> createdTextures;
};

