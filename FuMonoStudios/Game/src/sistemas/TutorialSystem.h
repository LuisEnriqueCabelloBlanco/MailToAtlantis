#pragma once
#include "../architecture/Entity.h"
#include "../components/DialogManager.h"
#include <functional>

namespace ecs {
	class TutorialScene;
}
class RenderImage;
class DialogManager;
class DialogComponent;

using SimpleCallback = std::function<void()>;

// Almacena el sistema de dialogo y además tiene un sistema de eventos
// que al pasarle un Enum TutorialEvent activa el evento y prohibe
// ciertas acciones y spawnea ciertos objetos para demostrar una mecánica
class TutorialSystem
{
public:
	enum TutorialEvent { Introduction, SacaElManual };

	TutorialSystem(ecs::TutorialScene* scene);
	~TutorialSystem();

	void activateEvent(TutorialEvent event);
	void stopEvent(TutorialEvent event);

	void closeConversation();

	void update();

	bool canDrag;
private:
	ecs::TutorialScene* scene_;

	// entidades de dialogo
	ecs::Entity* boxBackground_;
	ecs::Entity* textDialogue_;
	DialogManager dialogMngr_;
	void createDialogueBox();

	void activateDialogue(DialogManager::DialogSelection ds);
	int tutorialIteration;

	TutorialEvent currentEvent;

	// objetos de evento
	void createArrow();
	ecs::Entity* arrow_;

	// time en segundos
	void delayedCallback(float time, SimpleCallback call);
	SimpleCallback call_;
	Uint32 timer_;
	Uint32 timeToCall_;
	bool waitingCallback;
};

