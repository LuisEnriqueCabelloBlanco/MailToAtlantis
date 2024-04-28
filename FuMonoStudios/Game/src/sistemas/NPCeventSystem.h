#pragma once
#include <functional>
#include "json/JSONValue.h"
#include "../components/Paquete.h"
#include <sistemas/NPCevent.h>


using namespace pq;
class GeneralData;
class PaqueteBuilder;
class Game;

class NPCeventSystem
{
public:
	NPCeventSystem();
	virtual ~NPCeventSystem();

	void debugPaquetesInQueue();

	void activateEvent(NPCevent* e);

	bool areTherePaquetesNPC();
	Paquete* getPaqueteNPC();
	void addPaqueteNPC(Paquete* p);
	// comprueba si el paquete cumple la condicion de algun evento
	void checkPaqueteSent(Paquete* p);

	// llamar al final del minijuego y si se ha cumplido un evento, dar recompensas
	void minigameOver();

	// lee datos desde el json, llamado al crear generalData
	void readNPCEventData();

	void shuffleNPCqueue();
private:
	void procesarStringRecompensas(std::vector<std::string>& vec);

	void readNPCevent(JSONObject& eventObject, int personaje, int index);

	void readPaquetes(JSONObject& obj, NPCevent* auxEvent);
	void readPaquetesEspecificos(JSONObject& obj, NPCevent* event);

	Paquete* readPacage(JSONObject& obj);

	void readCondiciones(JSONObject& obj, NPCevent* auxEvent);
	void readCondicionesEspecificos(JSONObject& obj, NPCevent* auxEvent);

	std::vector<NPCevent*> activeEventsNPCs;
	std::vector<Paquete*> paquetesNPCs;


};

