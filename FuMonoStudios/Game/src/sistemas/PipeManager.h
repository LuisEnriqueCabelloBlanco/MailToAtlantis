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
	//Comprueba si el paquete es correcto (no hay fallos, está sellado, etc.)
		//Si es correcto comprueba que no se halla mandado a la papelera
			//Si no se ha mandado a la de retorno, comprueba que no incumpla otra infracción de la tubería destino:
			//Primero si está bloqueada
			//Luego si está intercambiado (si está cambiado se checkeará ese distrito a partir de ahora)
			//Luego, si hay algún tipo baneado
			//Por último, comprueba restricciones de peso
				//Si todo eso es correcto devuelve True :D
				//Else, devuelve false
			//Si se ha mandado a la papelera, comprueba (de manera similar a la de arriba) si las condiciones 
			//impedirían que se mandase el paquete.
				//Si lo impiden, es correcto devolverlo al remitente y devuelve True
				//Si no, es un fallo y devuelve false
		//Si el paquete es incorrecto comprueba que esté en la papelera
			//Si lo está, devuelve true
			//Si no, es un buen paquete en la basura y devuelve false D:

private:
	pq::Distrito returnPipe_;
	std::vector<bool> blockedPipes_;
	std::vector<SwappedPipe> swappedPipes_;
	std::vector<std::pair<bool, pq::TipoPaquete>> bannedTypePipes_;
	std::vector <WeightRestriction> weightRestrictionTypes_
};


