#pragma once
#include "../components/Paquete.h"
#include <vector>

struct SwappedPipe {
	//Comprueba si la tubería está cambiada
	bool swapActive;
	//Comprueba si la tubería también lleva a su destino original
	bool originalDis;
	//Distrito al que cambia (si está activo)
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
	//Comprueba si la tubería está bloqueada o cambiada
	bool checkPipeConditions(Paquete*, pq::Distrito);
	//Comprueba restricciones de peso y tipo
	bool checkPipeRestrictions(Paquete*, pq::Distrito);
	//Comprueba las restriciones de peso
	bool checkWeightRestrictions(Paquete*, pq::Distrito);
	//Comprueba si es imposible mandar un paquete correcto (por ejemplo, si su destino está bloqueado)
	bool checkReturningConditions(Paquete*);
	//Comprueba si alguna tuberia de las cambiadas es la del distrito del paquete
	bool checkSwappedPipes(Paquete*);

	pq::Distrito returnPipe_;
	std::vector<bool> blockedPipes_;
	std::vector<SwappedPipe> swappedPipes_;
	std::vector<std::pair<bool, pq::TipoPaquete>> bannedTypePipes_;
	std::vector <WeightRestriction> weightRestrictionTypes_;
};


