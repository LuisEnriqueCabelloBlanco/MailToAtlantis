#pragma once
#include <functional>
#include "../components/Paquete.h"

using Condition = std::function<bool(Paquete*)>;
using namespace pq;
class GeneralData;
class PaqueteBuilder;
class Game;

class NPCevent {
public:
	std::vector<std::string> recompensas;
	std::vector<Paquete*> paquetes;
	std::vector<std::vector<Condition>> condiciones;

	bool completed = false;
	int numPaquetes;
	int numPaquetesToComplete;

	bool usingDifConditions = false;
	void checkPaquete(Paquete* p) {
		if (!completed)
		{
			bool valido = true;
			int i = 0;
			
			if (usingDifConditions) {
				while (valido && i < condiciones.size()) {
					int j = 0;
					while (valido && i < condiciones.size()){
						valido = condiciones[i][j](p);
						j++;
					}
					i++;
				}
			}
			else {
				while (valido && i < condiciones[0].size()) {
					valido = condiciones[0][i](p);

					i++;
				}
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
	void procesarStringRecompensas(std::vector<std::string> vec);

	std::vector<NPCevent*> activeEventsNPCs;
	std::vector<Paquete*> paquetesNPCs;
};

