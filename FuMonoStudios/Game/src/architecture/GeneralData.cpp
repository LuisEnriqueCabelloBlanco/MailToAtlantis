#include "../components/DialogManager.h"
#include "GeneralData.h"
#include <iostream>
#include <string>
#include <fstream>
#include "../json/JSON.h"
#include "../json/JSONValue.h"
#include "../sdlutils/RandomNumberGenerator.h"
#include "../sistemas/PaqueteBuilder.h"
#include "Game.h"

GeneralData::GeneralData()
{
	// Esto habra que cargarlo desde un archivo de guardado
	dinero_ = INITIAL_MONEY;
	finalID_ = INITIAL_FINAL;
	failsMargin_ = INITIAL_FAILS_MARGIN;
	corrects_ = 0;
	fails_ = 0;
	dia_ = 1;
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
	// actualizar los datos para todos los npc
	for (int i = 0; i < 7; i++)
	{
		npcData[i]->setupDayData();
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
		std::string felicidadStr = jObject.find("Felicidad")->second->AsString();

		if (i < 2) // npc grandes
		{
			npcData.push_back(new NPCMayorData(stringToFelicidad(felicidadStr)));

			// leemos los eventos
			JSONObject eventObject = jObject["Eventos"]->AsObject();
			allEventsNPCs.emplace((Personaje)i, std::vector<NPCevent*>());

			// Hay 3 tipos de eventos. 
			// 1- Los especiales, que necesitan sus propias lineas
			// de codigo y se marcan poniendo un "special" y luego el numero de special.
			// Este tipo es util para paquetes con sprites especiales o los del dia 14.
			// 2- El que tiene los paquetes prehechos, marcado por un array "paquetes" que
			// hace los paquetes tal cual estan puestos. Este es util cuando los paquetes
			// son muy distintos.
			// 3- Y el condiciones, que hace que todos sus paquetes sean random, pero luego
			// añade sus condicones especiales. Esto es util cuando quieres que sean muy
			// similares los paquetes
			for (int j = 0; j < 3; j++)
			{
				NPCevent* auxEvent = new NPCevent();

				JSONObject currentEvent = eventObject.find(std::to_string(j + 1))->second->AsObject();
				auto isSpecial = currentEvent.find("special");
				if (isSpecial != currentEvent.end())
				{
					JSONObject obj = isSpecial->second->AsObject();
					// ES ESPECIAL
					int numSpecial = obj.find("special")->second->AsNumber();
					switch (numSpecial) {
					case 1:
						break;
					}
				}
				auto isPremadePaquete = currentEvent.find("paquetes");
				if (isPremadePaquete != currentEvent.end())
				{
					JSONObject obj = isPremadePaquete->second->AsObject();
					
					for (auto paq : obj) {
						JSONObject paqueteConditions = paq.second->AsObject();
						PaqueteBuilder paqBuild(Game::instance()->getScene(ecs::sc::MAIN_SCENE));
						
						std::string rem = paqBuild.remitenteRND();
						auto hasRemitente = paqueteConditions.find("remitente");
						if (hasRemitente != paqueteConditions.end())
							rem = hasRemitente->second->AsString();

						Distrito dist = (Distrito)sdlutils().rand().nextInt(0,getTubesAmount());
						auto hasDistrito = paqueteConditions.find("distrito");
						if (hasDistrito != paqueteConditions.end())
						{
							std::string aux = hasDistrito->second->AsString();
							if (aux == "Hestia")
								dist == Hestia;
							else if (aux == "Artemisa")
								dist == Artemisa;
							else if (aux == "Demeter")
								dist == Demeter;
							else if (aux == "Hefesto")
								dist == Hefesto;
							else if (aux == "Hermes")
								dist == Hermes;
							else if (aux == "Apolo")
								dist == Apolo;
							else if (aux == "Poseidon")
								dist == Poseidon;
							else if (aux == "Erroneo")
								dist == Erroneo;
						}

						Calle calle = (Calle)sdlutils().rand().nextInt(0, 3);
						auto hasCalle = paqueteConditions.find("calle");
						if (hasCalle != paqueteConditions.end())
						{

						}


						//Paquete* paquete = new Paquete();
					}
				}
			}

		}
		else
		{
			std::vector<bool> diasDanEventos;
			jObject = jValueRoot->AsObject();
			JSONObject jDiasEvento = jObject.find("DiasConEvento")->second->AsObject();
			// leemos los 14 booleanos
			for (int i = 0; i < 14; i++)
			{
				diasDanEventos.push_back(jDiasEvento.find(std::to_string(i + 1))->second->AsBool());
			}
			npcData.push_back(new NPCMenorData(stringToFelicidad(felicidadStr),diasDanEventos));
		}
		delete jValueRoot;
		jValueRoot = nullptr;
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

GeneralData::Personaje GeneralData::stringToPersonaje(const std::string& pers) {
	Personaje aux;
	// no deja hacer switch y es una cochinada pero es la unica forma de hacerlo
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

GeneralData::Felicidad GeneralData::stringToFelicidad(const std::string& str)
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

// Struct NPCdata
#pragma region NPCdata

GeneralData::NPCdata* GeneralData::getNPCData(Personaje personaje) {
	NPCdata* npc = nullptr;

	npc = npcData[personaje];

	return npc;
}

// NPC MENOR
GeneralData::NPCMenorData::NPCMenorData(Felicidad Felicidad, std::vector<bool> DiasDanEvento) {
	felicidad = Felicidad;
	iteration = 1;
	diasDanEvento = DiasDanEvento;
}

std::pair<const std::string, int> GeneralData::NPCMenorData::getDialogueInfo() {
	
	std::string tipo;
	int iterationNum = -1;

	if (felicidad == Minima || felicidad == Maxima || felicidad == NoHabladoAun)
	{
		switch (felicidad)
		{
		case NoHabladoAun:
			tipo = "Presentacion";
			felicidad = Normal;
			break;
		case Minima:
			tipo = "FelicidadMinimo";
			break;
		case Maxima:
			tipo = "FelicidadMaximo";
			break;
		}
	}
	else if (giveEvent)
	{
		tipo = "Eventos";
		RandomNumberGenerator a;
		iterationNum = a.nextInt(1, 6);
	}
	else
	{
		switch (felicidad){
			case Mala:
				tipo = "GenericoMalo";
				iterateDialogues();
				iterationNum = iteration;
				break;
			case Normal:
				tipo = "GenericoNormal";
				iterateDialogues();
				iterationNum = iteration;
				break;
			case Buena:
				tipo = "GenericoBueno";
				iterateDialogues();
				iterationNum = iteration;
				break;
		}
	}
	return std::make_pair(tipo, iterationNum);
}

void GeneralData::NPCMenorData::setupDayData() {
	iteration = 1;
	giveEvent = diasDanEvento[generalData().getDay() - 1];
}

void GeneralData::NPCMenorData::activateEvent(){
	giveEvent = true;
}

void GeneralData::NPCMenorData::deactivateEvent() {
	giveEvent = false;
}

void GeneralData::NPCMenorData::iterateDialogues() {
	iteration++;
	if (iteration > 3)
		iteration = 1;
}

// NPC GRANDE

GeneralData::NPCMayorData::NPCMayorData(Felicidad Felicidad) {
	felicidad = Felicidad;
	postConversation = false;
}

std::pair<const std::string, int> GeneralData::NPCMayorData::getDialogueInfo() {
	std::string aux;

	switch (felicidad)
	{
		case NoHabladoAun:
			aux = "Presentacion";
			felicidad = Normal;
			break;
		case Minima:
			aux = "FelicidadMinimo";
			break;
		default:
			aux = postConversation ?
				"PostConversacionDia" : "Dia";
			aux = aux + std::to_string(generalData().getDay());
			postConversation = true;
			break;
	}

	return std::make_pair(aux, -1);
}

void GeneralData::NPCMayorData::setupDayData() {
	postConversation = false;
}
#pragma endregion