#include "../components/DialogManager.h"
#include "GeneralData.h"
#include <iostream>
#include <string>
#include <fstream>
#include "../json/JSON.h"
#include "../json/JSONValue.h"
#include "../sdlutils/RandomNumberGenerator.h"

GeneralData::GeneralData()
{
	// Esto habra que cargarlo desde un archivo de guardado
	dinero_ = INITIAL_MONEY;
	finalID_ = INITIAL_FINAL;
	eventoID_ = INITIAL_EVENT; 
	failsMargin_ = INITIAL_FAILS_MARGIN;
	corrects_ = 0;
	fails_ = 0;
	dia_ = INITIAL_DAY;
	rent_ = 75;
	numTubos_ = INITIAL_TUBE_AMOUNT;

	readNPCData();
}

GeneralData::~GeneralData() {

}


void GeneralData::updateMoney()
{
	int rightPackages = corrects_;
	int wrongPackages = fails_;
	//funcion de ejemplo seguramente haya que cambiarlo
	if (fails_ < failsMargin_) {
		wrongPackages = 0;
	}
	if (corrects_ < 0) {
		rightPackages = 0;
	}
	dinero_ += (rightPackages * WRITE_PACAGES_VALUE) - (wrongPackages * WRONG_PACAGES_VALUE) - rent_;
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

void GeneralData::setRent(int rent) {
	rent_ = rent;
	std::cout << "el nuevo alquiler es: " << rent_ << std::endl;
}

int GeneralData::getRent() {

	return rent_;

}

void GeneralData::updateDia()
{
	placesToActive_.clear();
	updateDistrictsPerDay(dia_);
}

std::string GeneralData::fromDistritoToString(int i) {

	std::string sol;

	switch (i) {
		case Distrito::Hestia:
			sol = "Hestia";
			break;
		case Distrito::Artemisa:
			sol = "Artemisa";
			break;
		case Distrito::Demeter:
			sol = "Demeter";
			break;
		case Distrito::Hefesto:
			sol = "Hefesto";
			break;
		case Distrito::Hermes:
			sol = "Hermes";
			break;
		case Distrito::Apolo:
			sol = "Apolo";
			break;
		case Distrito::Poseidon:
			sol = "Poseidon";
			break;
		default:
			sol = "Erroneo";
			break;
	}

	return sol;

}

int GeneralData::fromStringToDistrito(std::string place) {

	int sol;

	if (place == "Hestia") {

		return Distrito::Hestia;

	}
	else if (place == "Artemisa") {

		return Distrito::Artemisa;

	}
	else if (place == "Demeter") {

		return Distrito::Demeter;

	}
	else if (place == "Hefesto") {

		return Distrito::Hefesto;

	}
	else if (place == "Hermes") {

		return Distrito::Hermes;

	}
	else if (place == "Apolo") {

		return Distrito::Apolo;

	}
	else if (place == "Poseidon") {

		return Distrito::Poseidon;

	}
	else {

		return Distrito::Erroneo;

	}

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

void GeneralData::readNPCData() {
	std::unique_ptr<JSONValue> jsonFile(JSON::ParseFromFile("recursos/data/npcData.json"));

	if (jsonFile == nullptr || !jsonFile->IsObject()) {
		throw "Something went wrong while load/parsing npcData";
	}
	
	JSONObject root = jsonFile->AsObject();
	JSONValue* jValueRoot = nullptr;

	// cargamos los 7 personajes

	for (int i = 0; i < 7; i++)
	{
		std::string aux = personajeToString((Personaje)i);
		jValueRoot = root[aux];

		JSONObject jObject = jValueRoot->AsObject();
		std::string felicidadStr = jObject["Felicidad"]->AsString();

		if (i < 2) // npc grandes
		{
			npcData[(Personaje)i] = new NPCMayorData(stringToFelicidad(felicidadStr));
		}
		else
		{
			std::vector<bool> diasDanEventos;
			jObject = jValueRoot->AsObject();
			JSONObject jDiasEvento = jObject["DiasConEvento"]->AsObject();
			// leemos los 14 booleanos
			for (int i = 0; i < 14; i++)
			{
				diasDanEventos.push_back(jDiasEvento.find(std::to_string(i + 1))->second->AsBool());
			}
			npcData[(Personaje)i] = new NPCMenorData(stringToFelicidad(felicidadStr),diasDanEventos);
		}
		
	}
	
}

void GeneralData::writeNPCData() {

}

const std::string GeneralData::personajeToString(Personaje pers) {

	std::string aux = "";
	switch (pers) {
		case Vagabundo:
			aux = "Vagabundo";
			break;
		case Secretario:
			aux = "Secretario";
			break;
		case Campesino:
			aux = "Campesino";
			break;
		case Artesano:
			aux = "Artesano";
			break;
		case Tarotisa:
			aux = "Tarotisa";
			break;
		case Soldado:
			aux = "Soldado";
			break;
		case Contable:
			aux = "Contable";
			break;
	}
	return aux;
}

Personaje GeneralData::stringToPersonaje(const std::string& pers) {
	Personaje aux;
	// no deja hacer switch y es una cochinada pero es la unica forma de hacerlo
	//se puede usar un hasmap
	if (pers == "Vagabundo")
		aux = Vagabundo;
	else if (pers == "Secretario")
		aux = Secretario;
	else if (pers == "Campesino")
		aux = Campesino;
	else if (pers == "Artesano")
		aux = Artesano;
	else if (pers == "Tarotisa")
		aux = Tarotisa;
	else if (pers == "Soldado")
		aux = Soldado;
	else if (pers == "Contable")
		aux = Contable;
	
	return aux;
}

Felicidad GeneralData::stringToFelicidad(const std::string& str)
{
	Felicidad aux;
	if (str == "Minima")
		aux = Felicidad::Minima;
	else if (str == "Mala")
		aux = Felicidad::Mala;
	else if (str == "Normal")
		aux = Felicidad::Normal;
	else if (str == "Buena")
		aux = Felicidad::Buena;
	else if (str == "Maxima")
		aux = Felicidad::Maxima;
	else if (str == "NoHabladoAun")
		aux = Felicidad::NoHabladoAun;

	return aux;
}

void GeneralData::setDayData() {
	for (int i = 0; i < 7; i++)
	{
		npcData.at((Personaje)i)->setupDayData();
	}
}

// Struct NPCdata
#pragma region NPCdata

NPCdata* GeneralData::getNPCData(Personaje personaje) {
	NPCdata* npc = nullptr;

	npc = npcData[personaje];

	return npc;
}


#pragma endregion