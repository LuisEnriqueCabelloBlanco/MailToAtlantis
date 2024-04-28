#pragma once
#include <utils/checkML.h>
#include <functional>
#include <components/Paquete.h>
using Condition = std::function<bool(Paquete*)>;

class NPCevent {
public:
	NPCevent();
	virtual ~NPCevent();
	std::vector<std::string> recompensas;
	std::vector<Paquete*> paquetes;
	std::vector<std::vector<Condition>> condiciones;

	bool completed;
	int numPaquetes;
	int numPaquetesToComplete;

	bool usingDifConditions;
	void checkPaquete(Paquete* p);

	void paqueteSuccesful();
	int paquetesDone;
};