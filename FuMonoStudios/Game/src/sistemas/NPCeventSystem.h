#pragma once
#include <functional>
#include "../components/Paquete.h"

using Condition = std::function<bool(Paquete*)>;
using namespace pq;
class GeneralData;
class PaqueteBuilder;
class Game;

struct NPCevent {
	std::vector<std::string> recompensas;
	std::vector<Paquete*> paquetes;
	std::vector<Condition> condiciones;

	bool completed = false;
	int numPaquetes;
	int numPaquetesToComplete;

	void checkPaquete(Paquete* p) {
		if (!completed)
		{
			bool valido = false;
			int i = 0;
			while (!valido && i < condiciones.size()) {
				valido = condiciones[i](p);
				i++;
			}

			if (valido)
				paqueteSuccesful();
		}
	}

	void paqueteSuccesful() {
		paquetesDone++;
		completed = paquetesDone >= numPaquetesToComplete;
	};
	int paquetesDone = 0;
};


class NPCeventSystem
{
public:
	NPCeventSystem();
	~NPCeventSystem();

	bool areTherePaquetesNPC();
	Paquete* getPaqueteNPC();
	// comprueba si el paquete cumple la condicion de algun evento
	void checkPaqueteSent(Paquete* p);

	void addActiveEvent(NPCevent* event);

	// llamar al final del minijuego y si se ha cumplido un evento, dar recompensas
	void minigameOver();

	// lee datos desde el json, llamado al crear generalData
	void readNPCEventData();

	void shuffleNPCqueue();
private:
	void procesarStringRecompensas(std::vector<std::string> vec);

	std::vector<NPCevent*> activeEventsNPCs;
	std::vector<Paquete*> paquetesNPCs;
};

