#pragma once
#include "Paquete.h"
#include "../architecture/Entity.h"
#include <stdlib.h>


const int pesadoMax = 75;	//L�mite del peso m�ximo de paquetes pesados 
const int medioMax = 50;	//L�mite del peso m�ximo de paquetes de peso medio 
const int ligeroMax = 25;	//L�mite del peso m�ximo de paquetes ligeros
const int pesoCartaMax = 2;	//L�mite del peso m�ximo de cartas

class PaqueteBuilder
{
private:
	void Nivel0(ecs::Entity* ent) {	//Un paquete que no tiene ni sellos normales, de peso o de fragil, y solo puede tener calles err�neas
		
		Paquete* pq = ent->addComponent<Paquete>(DistritoRND(),CalleRND(35), TipoRND(), true, Paquete::NivelPeso::Ninguno, rand() % pesadoMax + 1, false,false);
		std::cout << "El paquete es parte del distrito " << pq->getDist() << "\n";
	}
	void Nivel1(ecs::Entity* ent) {	//Un paquete que no tiene ni sellos de peso ni sello de fragil, y puede tener tanto calles como sellos de tipo err�neos
		Paquete* pq = ent->addComponent<Paquete> (DistritoRND (), CalleRND (20), TipoRND (), BoolRND (35), Paquete::NivelPeso::Ninguno, rand () % pesadoMax + 1, false, false);
	}
	void Nivel2(ecs::Entity* ent) { //Un paquete que no tiene sello de fragil, pero puede tener sellos de peso, as� como calles err�neas y sellos de tipo err�neos
		int peso;
		Paquete::NivelPeso Nv = PesoRND (25, 30, peso);
		Paquete* pq = ent->addComponent<Paquete> (DistritoRND (), CalleRND (15), TipoRND (), BoolRND (20), Nv, peso, false, false);
	}
	void Nivel3(ecs::Entity* ent) { //Un paquete que puede tener peso, sellos de fr�gil, calles err�neas y sellos de tipo err�neos
		int peso;
		Paquete::NivelPeso Nv = PesoRND(20, 25, peso);
		Paquete* pq = ent->addComponent<Paquete> (DistritoRND (), CalleRND (15), TipoRND (), BoolRND (20), Nv, peso, BoolRND (80), false);		
	}
	void Carta(ecs::Entity* ent) {	//Una carta, que en esencia funciona igual que un paquete de nivel 0
		Paquete* pq = ent->addComponent<Paquete> (DistritoRND (), CalleRND (35), TipoRND (), true, Paquete::NivelPeso::Ninguno, rand () % pesoCartaMax + 1, false, true);		
	}

	Paquete::Distrito DistritoRND();	//M�todo que elige un distrito aleatorio de los que hay
	Paquete::TipoPaquete TipoRND();		//M�todo que elige un tipo de paquete aleatorio entre los que hay
	Paquete::Calle CalleRND(int probError);	//M�todo que elige una calle aleatoria de las posibilidades. El valor probError es, sobre 100, la probabilidad de que sea una calle incorrecta
	bool BoolRND(int probFalse);		//M�todo que genera un bool con valor aleatorio entre true y false. El valor probFalse es, sobre 100, la probabilidad de que sea false
	Paquete::NivelPeso PesoRND(int probPeso, int probError, int& peso);	//M�todo que elige si un paquete tiene peso, y si es err�neo, devolviendo un peso para el paquete con la variable "peso"

public:
	//M�todo al que se llama que devuelve un Paquete generado aleatoriamente 
	void PaqueteRND(int level, ecs::Entity *ent) {
		
		if (level == 0) {
			Nivel0(ent);
		}
		else if (level == 1) {
			Nivel1(ent);
		}
		else if (level == 2) {
			Nivel2(ent);
		}
		else if (level == 3) {
			Nivel3(ent);
		}				
	}
	//M�todo al que se llama que devuelve una Carta generada aleatoriamente 
	Paquete* CartaRND(ecs::Entity* ent) {
		Carta(ent);
	}

};