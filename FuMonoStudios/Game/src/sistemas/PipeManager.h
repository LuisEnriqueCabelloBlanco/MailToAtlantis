#pragma once
#include "../components/Paquete.h"
#include <vector>

struct SwappedPipe {
	//Comprueba si la tuber�a est� cambiada
	bool swapActive;
	//Comprueba si la tuber�a tambi�n lleva a su destino original
	bool originalDis;
	//Distrito al que cambia (si est� activo)
	pq::Distrito changedDis;
};

struct WeightRestriction {
	//Checks if the weight is restricted
	bool weightRestricted;
	//Checks if only one type is restricted
	bool singleType;
	//0 if weight has to be higher than x, 1 if it needs to be x, 2 if it needs to be lower than x, 
	//3 if it can be any weight except x
	int minOrMax;
	//Peso comprobado
	pq::NivelPeso x;
	//Si solo comprueban pesos de un tipo, el tipo
	pq::TipoPaquete typeToWeight;
};

class PipeManager
{
public:
	PipeManager();
	~PipeManager();

	void init();

	void updateConditions();

	bool checkPackage(Paquete*, pq::Distrito);

private:
	bool checkConditions(Paquete*, pq::Distrito);

	pq::Distrito returnPipe_;
	std::vector<bool> blockedPipes_;
	std::vector<SwappedPipe> swappedPipes_;
	std::vector<std::pair<bool, pq::TipoPaquete>> bannedTypePipes_;
	std::vector <WeightRestriction> weightRestrictionTypes_;
};


