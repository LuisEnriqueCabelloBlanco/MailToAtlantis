#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <functional>
#include <components/Paquete.h>
using Condition = std::function<bool(Paquete*)>;
using ConditionTubo = std::function<bool(Distrito)>;

class NPCevent {
public:
	NPCevent();
	~NPCevent();

	std::vector<std::string> recompensas;
	std::vector<Paquete*> paquetes;
	std::vector<std::vector<Condition>> condiciones;
	ConditionTubo condicionTubo;
	bool usingCondicionTubo = false;

	std::string textoDiario;

	bool completed = false;
	int numPaquetes;
	int numPaquetesToComplete;

	bool usingDifConditions = false;
	void checkPaquete(Paquete* p);

	void paqueteSuccesful();
	int paquetesDone = 0;

	Personaje personaje;
	int numEvento;
};