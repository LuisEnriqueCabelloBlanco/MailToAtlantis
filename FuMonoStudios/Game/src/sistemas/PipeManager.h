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
	//Comprueba si el paquete es correcto (no hay fallos, est� sellado, etc.)
		//Si es correcto comprueba que no se halla mandado a la papelera
			//Si no se ha mandado a la de retorno, comprueba que no incumpla otra infracci�n de la tuber�a destino:
			//Primero si est� bloqueada
			//Luego si est� intercambiado (si est� cambiado se checkear� ese distrito a partir de ahora)
			//Luego, si hay alg�n tipo baneado
			//Por �ltimo, comprueba restricciones de peso
				//Si todo eso es correcto devuelve True :D
				//Else, devuelve false
			//Si se ha mandado a la papelera, comprueba (de manera similar a la de arriba) si las condiciones 
			//impedir�an que se mandase el paquete.
				//Si lo impiden, es correcto devolverlo al remitente y devuelve True
				//Si no, es un fallo y devuelve false
		//Si el paquete es incorrecto comprueba que est� en la papelera
			//Si lo est�, devuelve true
			//Si no, es un buen paquete en la basura y devuelve false D:

private:
	pq::Distrito returnPipe_;
	std::vector<bool> blockedPipes_;
	std::vector<SwappedPipe> swappedPipes_;
	std::vector<std::pair<bool, pq::TipoPaquete>> bannedTypePipes_;
	std::vector <WeightRestriction> weightRestrictionTypes_
};


