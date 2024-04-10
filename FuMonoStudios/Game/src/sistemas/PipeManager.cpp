#include "PipeManager.h"

PipeManager::PipeManager()
{

}

PipeManager::~PipeManager()
{

}

void PipeManager::init()
{

}

void PipeManager::updateConditions()
{

}

bool PipeManager::checkPackage(Paquete* pqt, pq::Distrito toDis)
{
	//Comprueba si el paquete es correcto de base
		//Si lo es, se comprueba que se haya procesado bien (sellar calle, evolver, etc)
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
	bool correct = pqt->correcto();

	if (correct) {
		//correct = pqt->bienSellado() && pqt->correctFragile() && checkConditions(pqt, toDis);
		if (pqt->bienSellado() && pqt->correctFragile()) {
			if (toDis == returnPipe_) {
				correct = !checkConditions(pqt, toDis);
			}
			else {
				correct = checkConditions(pqt, toDis);
			}
		}
		else {
			correct = false;
		}
	}
	else {
		if (toDis == returnPipe_) {
			correct = true;
		}
		else {

		}
	}

	return correct;
}
