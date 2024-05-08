#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <architecture/Entity.h>
#include <components/DialogManager.h>
#include <functional>

namespace ecs {
	class TutorialScene;
}
class RenderImage;
class DialogManager;
class DialogComponent;
class MoverTransform;
class Wrap;

using SimpleCallback = std::function<void()>;

// Este sistema se divide en dos partes:
// El activateEvent y el stopEvent: El activateEvent activa el evento y llama
// a activateDialogue, que muestra un texto y al terminar, llama a su correspondiente
// stopEvent.
// Para poder ir avanzando y saber que el jugador ha hecho acciones, se ha modificado
// los elementos de la oficina para que al hacer acciones llamen a 'registerAction'
// que en caso de tener un metodo de listener, recibira la accion.
//
// La forma de funcionar de un evento es: Ser llamado en activateEvent y sacar su 
// dialogo, luego al acabar el dialogo, se llama al stopEvent y alli se coloca una
// condicion para avanzar al siguiente evento, que puede ser delayedCallback o 
// eventListener.
class TutorialSystem
{
public:
	enum TutorialEvent { Introduction, SacaElManual1, SacaElManual2, PaqueteEnsenarRemitente,
	PaqueteEnsenarCodigoPostal, PaqueteBuscarPaginaCodigosPostales, BuscarPaginaHestia,
	EnsenarSellos, EnsenarTubos, EntraSegundoPaquete, SegundoBuscarPaginaDistritos, 
	SellarSegundoPaquete, EnviarSegundoPaquete, EntraCuartoPaquete, ExplicacionFalloAposta, 
	EntraTercerPaquete, EnPaginaInfoSellos,
	EntraPaquetePeso, PesarPaquetePeso, EnviarPaquetePeso, 
	EntraPaqueteFragil, PaginaFragil, SellarFragil, EnviarFragil,
	Fin};

	enum Action { SacarManual, PaginaCodigosPostales, PaginaDistritoHestia, PaqueteEstampado,
	PaqueteEnviado, PaginaDistritoDemeter, PaginaSellos, Basura, Embalado, Pesado, PaginaPesado, PaginaFragilAccion};

	TutorialSystem(ecs::TutorialScene* scene);
	~TutorialSystem();

	void registerAction(Action a);

	void addActionListener(Action a, SimpleCallback call);

	void activateEvent(TutorialEvent event);
	void stopEvent(TutorialEvent event);

	void init();

	void update();

	bool canDrag;
	bool canPassPagesManual;
private:

	ecs::TutorialScene* scene_;

	DialogManager dialogMngr_;

	void activateDialogue(bool dialogBoxInBottom);
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

	Wrap* waitingWrapComp;
	bool waitingEmbalaje;

	bool waitingPesado;
};

