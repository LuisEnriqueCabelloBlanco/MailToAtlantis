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
class MoverTransform;

using SimpleCallback = std::function<void()>;

// Almacena el sistema de dialogo y además tiene un sistema de eventos
// que al pasarle un Enum TutorialEvent activa el evento, prohibe
// ciertas acciones y spawnea ciertos objetos para demostrar una mecánica
//
// El registerAction registra las acciones que va haciendo el jugador para poder
// pasar de un evento a otro
class TutorialSystem
{
public:
	enum TutorialEvent { Introduction, SacaElManual1, SacaElManual2, PaqueteEnseñarRemitente,
	PaqueteEnseñarCodigoPostal, PaqueteBuscarPaginaCodigosPostales, PaqueteBuscarPaginaHestia,
	PaqueteEnseñarSellos};

	enum Action { SacarManual, PaginaCodigosPostales, PaginaDistritoHestia };

	TutorialSystem(ecs::TutorialScene* scene);
	~TutorialSystem();

	void registerAction(Action a);

	void addActionListener(Action a, SimpleCallback call);

	void activateEvent(TutorialEvent event);
	void stopEvent(TutorialEvent event);

	void closeConversation();

	void update();

	bool canDrag;
	bool canPassPagesManual;
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

	std::vector<std::pair<Action, SimpleCallback>> actionListeners;

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

