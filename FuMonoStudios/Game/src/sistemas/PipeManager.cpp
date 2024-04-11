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
	bool correct = pqt->correcto();

	if (correct) {
		//correct = pqt->bienSellado() && pqt->correctFragile() && checkConditions(pqt, toDis);
		if (pqt->bienSellado() && pqt->correctFragile()) {
			if (toDis == returnPipe_) {
				//correct = !checkConditions(pqt, toDis);
			}
			else {
				correct = checkPipeConditions(pqt, toDis);
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

bool PipeManager::checkPipeConditions(Paquete* pqt, pq::Distrito toDis)
{
	bool correct;
	SwappedPipe it = swappedPipes_[toDis];

	/*
	if (!blockedPipes_[toDis]) {
		if (it.swapActive) {
			if (it.changedDis == pqt->getDistrito()) {
				correct = checkPipeRestrictions(pqt, it.changedDis);
			}
			else if (it.originalDis && toDis == pqt->getDistrito()) {
				correct = checkPipeRestrictions(pqt, toDis);
			}
			else {
				correct = false;
			}
		}
		else {
			if (toDis == pqt->getDistrito()) {
				correct = checkPipeRestrictions(pqt, toDis);
			}
			else {
				correct = false;
			}
		}
	}
	else {
		correct = false;
	}*/

	return (!blockedPipes_[toDis] && ((it.swapActive && 
			((it.changedDis == pqt->getDistrito() && checkPipeRestrictions(pqt, it.changedDis)) || 
			(it.originalDis && toDis == pqt->getDistrito() && checkPipeRestrictions(pqt, toDis)))) || 
			(!it.swapActive && toDis == pqt->getDistrito() && checkPipeRestrictions(pqt, toDis))));
}

bool PipeManager::checkPipeRestrictions(Paquete* pqt, pq::Distrito toDis)
{
	/*
	if (bannedTypePipes_[toDis].first && pqt->getTipo() == bannedTypePipes_[toDis].second) {
		correct = false;
	}
	else {
		correct = checkWeightRestrictions(pqt, toDis);
	}*/

	return (!(bannedTypePipes_[toDis].first && pqt->getTipo() == bannedTypePipes_[toDis].second) && checkWeightRestrictions);
}

bool PipeManager::checkWeightRestrictions(Paquete* pqt, pq::Distrito toDis)
{
	bool correct;
	WeightRestriction it = weightRestrictionTypes_[toDis];
	pq::NivelPeso peso = pqt->getPeso();
	
	if (it.weightRestricted && peso != pq::Ninguno) {
		if (it.singleType) {
			if (pqt->getTipo() != it.typeToWeight) {
				correct = true;
			}
			else {
				if (it.minOrMax == 0 && it.x < peso) {
					correct = true;
				}
				else if (it.minOrMax == 1 && it.x == peso) {
					correct = true;
				}
				else if (it.minOrMax == 2 && it.x > peso) {
					correct = true;
				}
				else if (it.minOrMax == 3 && it.x != peso) {
					correct = true;
				}
				else {
					correct = false;
				}
			}
		}
		else {
			if (it.minOrMax == 0 && it.x < peso) {
				correct = true;
			}
			else if (it.minOrMax == 1 && it.x == peso) {
				correct = true;
			}
			else if (it.minOrMax == 2 && it.x > peso) {
				correct = true;
			}
			else if (it.minOrMax == 3 && it.x != peso) {
				correct = true;
			}
			else {
				correct = false;
			}
		}
	}
	else {
		correct = true;
	}

	return correct;
}
