#include "GeneralData.h"
#include <iostream>
#include <string>

void GeneralData::updateMoney(int correct, int wrong)
{
	int rightPackages = correct;
	corrects_ = correct;
	int wrongPackages = wrong;
	fails_ = wrong;
	//funcion de ejemplo seguramente haya que cambiarlo
	if (wrong < failsMargin_) {
		wrongPackages = 0;
	}
	if (correct < 0) {
		rightPackages = 0;
	}
	dinero_ += rightPackages * WRITE_PACAGES_VALUE - wrongPackages * WRONG_PACAGES_VALUE;
}

//A medida que el proyecto avance, la lista de variables deber� de ampliarse, pero por ahora tenemos esto:

void GeneralData::setFinalID(int final) {
	finalID_ = final;
	std::cout << "El ID del final del juego es: " << finalID_ << std::endl;
}

int GeneralData::getFinalID() {
	std::cout << "El ID del final del juego que quieres obtener es: " << finalID_ << std::endl;
	return finalID_;
}

void GeneralData::setEventoID(int evento) {
	eventoID_ = evento;
	std::cout << "El ID del evento es: " << eventoID_ << std::endl;
}

int GeneralData::getEventoID() {
	std::cout << "El ID del evento que quieres obtener es: " << eventoID_ << std::endl;
	return eventoID_;
}

void GeneralData::updateDia()
{
	updateDistrictsPerDay(dia_);
}

void GeneralData::updateDistrictsPerDay(int dia)
{
	if(dia == 0)
		return;

	switch (dia)
	{
	case 1:
		//tutorial
		placesToActive_.push_back("Hestia");
		placesToActive_.push_back("Hefesto");
		placesToActive_.push_back("Demeter");
		placesToActive_.push_back("Artemisa");
		break;
	case 2:
		placesToActive_.push_back("Hermes");
		break;
	case 3: 
		placesToActive_.push_back("Apolo");
		break;
	case 4:
		placesToActive_.push_back("Poseidon");
		break;
	default:
		break;
	}
	updateDistrictsPerDay(dia - 1);
}

int GeneralData::getPaqueteLevel() {
	//Aqui habra que decidir el paquete level en funci�n del d�a
	return paqueteLvl_;
}

void GeneralData::setPaqueteLevel(int lvl) {
	paqueteLvl_ = lvl;
}
