#include "Paquete.h"
#include <stdlib.h>
const int nivelFragil = 3;
const int nivelPeso = 2;
const int nivelSellos = 1;

const int desviacionPeso = 15;
const int ligeroMax = 25;
const int medioMax = 50;
const int pesadoMax = 75;


Paquete::Paquete(int level) {	
	//El int "level" indica c�mo se debe generar un paquete seg�n el progreso del juego, ya que se van introduciendo mec�nicas nuevas a medida que progresa la semana

	int probManager1; //Estas dos variables son las que se deber�n modificar para cambiar la probabilidad de que los paquetes sean err�neos o correctos
	int probManager2;

	int rnd; //Variable que se usar� para aleatorizar

	//Si es fr�gil
	if (level >=nivelFragil) { //Si el nivel es lo suficientemente alto como para que haya el sobre de fr�gil

		rnd = rand() % 4;	//Genera un n�mero aleatorio entre 0 y 3
		if (rnd <= 0) fragil = true; //Hay un 25% de probabilidad de que sea un paquete fr�gil
		else fragil = false;
	}
	else fragil = false;

	//Si hay que pesar
	if (level >= nivelPeso) { //Si el nivel es lo suficientemente alto como para que el paquete pueda tener peso
		rnd = rand() % 9;	//Genera un n�mero aleatorio entre 0 y 8
		if (rnd < 4) miPeso = (NivelPeso)rnd; // Hay 3 posibilidades sobre 9 (33%) de probabilidad de que tenga peso
		else miPeso = Ninguno;

		//Y aqu� se le da un valor al peso, con cierto valor de desviaci�n para que haya alguna probabilidad de que sea err�neo
		if (miPeso == Bajo) {
			peso = rand() % (ligeroMax+ desviacionPeso);
		}
		else if (miPeso == Medio) {
			peso = rand() % (medioMax + desviacionPeso - ligeroMax) + ligeroMax - desviacionPeso / 2;
		}
		else if (miPeso == Alto) {
			peso = rand() % (pesadoMax + desviacionPeso - medioMax) + medioMax - desviacionPeso;
		}
		else if (miPeso == Ninguno) {
			peso = rand() % pesadoMax; //Se le da un valor al peso aunque no fuera a tener sello, por si el jugador quiere pesar el paquete de todas formas
		}
	}
	else miPeso = Ninguno;

	rnd = rand() % 5;	//Coloca un tipo de contenido aleatorio
	miTipo = (TipoPaquete) rnd;

	//Si hay que comprobar los sellos
	if (level >= nivelSellos) {
		rnd = rand() % 4;	//Genera un n�mero aleatorio entre 0 y 3
		if (rnd == 0) selloCorrecto = false; //Hay un 25% de probabilidad de que sea un paquete con un sello falso
		else selloCorrecto = true;
	}
	else selloCorrecto = true;

	rnd = rand() % 7;	//Coloca un tipo de distrito aleatorio
	miDistrito = (Distrito)rnd;
	
	rnd = rand() % 4;	//Coloca un tipo de calle alatorio
	miCalle = (Calle)rnd; //Hay un 25% de probabilidad de que sea un paquete con una direcci�n err�nea
}

Paquete::~Paquete() {

}

bool Paquete::Correcto() const{ 
	//M�todo que comprueba si el paquete hab�a sido generado sin errores (AKA: Si da false, eso significa que se tendr�a que devolver al remitente)
	bool resul = true;
	if (miCalle == Erronea) {
		resul = false;
	}
	else if (!selloCorrecto) {
		resul = false;
	}
	else if (miPeso != Ninguno){
		if (miPeso == Bajo) {
			if (peso > ligeroMax) resul = false;
		}
		else if (miPeso == Medio) {
			if (peso < ligeroMax || peso > medioMax) resul = false;
		}
		else if (miPeso == Alto) {
			if (peso < medioMax) resul = false;
		}		
	}
	return resul;
}

void Paquete::sellarCalle(Calle sello) {
	if (sello != Erronea && calleMarcada == Erronea) // solo puedes sellar una vez
	{
		calleMarcada = sello;
	}
}