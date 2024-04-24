#include "../components/DialogManager.h"
#include "GeneralData.h"
#include "../json/JSON.h"
#include "../json/JSONValue.h"
#include "../sdlutils/RandomNumberGenerator.h"
#include "../sistemas/PaqueteBuilder.h"
#include "Game.h"
#include "../sistemas/NPCeventSystem.h"


GeneralData::GeneralData()
{
	// Esto habra que cargarlo desde un archivo de guardado
	dinero_ = INITIAL_MONEY;
	finalID_ = INITIAL_FINAL;
	failsMargin_ = INITIAL_FAILS_MARGIN;
	corrects_ = 0;
	fails_ = 0;
	dia_ = INITIAL_DAY;
	rent_ = 75;
	numTubos_ = INITIAL_TUBE_AMOUNT;

}

GeneralData::~GeneralData() {
	delete npcEventSys;
}

void GeneralData::loadSaveFile()
{
	std::unique_ptr<JSONValue> jsonFile(JSON::ParseFromFile("recursos/data/saveFile.json"));

	JSONObject root = jsonFile->AsObject();

	dia_ = root.find("Dia")->second->AsNumber();
	dinero_ = root.find("Dinero")->second->AsNumber();
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
	dinero_ += calcularDineroGanado() - rent_;
}

int GeneralData::calcularDineroGanado()
{
	int rightPackages = corrects_;
	int wrongPackages = fails_;
	return (rightPackages * WRITE_PACAGES_VALUE) - (wrongPackages * WRONG_PACAGES_VALUE);
}

void GeneralData::resetMoney()
{
	dinero_ = INITIAL_MONEY;
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
	for (auto it : npcData)
	{
		it.second->setupDayData();
	}

	if (dia_ == 1) {
		dinero_ = INITIAL_MONEY;
	}
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
	case 5:
		placesToActive_.push_back("Hermes");
		placesToActive_.push_back("Apolo");
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
	JSONValue* jValue = nullptr;

	// cargamos los 7 personajes

	for (int i = 0; i < 7; i++)
	{
		std::string aux = personajeToString((Personaje)i);
		jValue = root[aux];

		JSONObject jObject = jValue->AsObject();
		std::string felicidadStr = jObject["Felicidad"]->AsString();


		if (i < 2) // npc grandes
		{
			NPCMayorData* data = new NPCMayorData(stringToFelicidad(felicidadStr));
			data->numMisionesAceptadas = jObject.find("numMisionesAceptadas")->second->AsNumber();
			data->numFelicidad = jObject.find("FelicidadNum")->second->AsNumber();
			npcData.emplace((Personaje)i,data);
		}
		else
		{
			std::vector<bool> diasDanEventos;
			jObject = jValue->AsObject();
			JSONObject jDiasEvento = jObject.find("DiasConEvento")->second->AsObject();

			// leemos los 14 booleanos
			for (int j = 0; j < 14; j++)
			{
				diasDanEventos.push_back(jDiasEvento.find(std::to_string(j + 1))->second->AsBool());
			}
			NPCMenorData* data = new NPCMenorData(stringToFelicidad(felicidadStr), diasDanEventos);
			data->numMisionesAceptadas = jObject.find("numMisionesAceptadas")->second->AsNumber();
			data->numFelicidad = jObject.find("FelicidadNum")->second->AsNumber();
			npcData.emplace((Personaje)i,data);
		}
		jValue = nullptr;
	}

	if (npcEventSys == nullptr)
		npcEventSys = new NPCeventSystem();
}

void GeneralData::writeNPCData() {
	std::ifstream archivo("recursos/data/npcData.json");

	if (!archivo.is_open())
	{
		std::cout << "Error al abrir npcData.json" << std::endl;
		throw std::runtime_error("Error al abrir npcData.json");
	}

	// Leer el contenido del archivo en una cadena
	std::string contenido = "";
	std::string linea;
	while (std::getline(archivo, linea)) {
		contenido += linea + "\n";
	}
	archivo.close();


	for (int i = 0; i < 7; i++)
	{
		NPCdata* data = getNPCData((Personaje)i);
		int posPersonaje = contenido.find('"' + personajeToString((Personaje)i) + '"' + ':');

		int posFelicidad = contenido.find("Felicidad", posPersonaje) + 12;
		contenido.replace(posFelicidad, (contenido.find('\n', posFelicidad)) - posFelicidad,
			'"' + generalData().felicidadToString(data->felicidad) + '"' + ',');
		int posFelicidadNum = contenido.find("FelicidadNum", posPersonaje) + 15;
		contenido.replace(posFelicidadNum, (contenido.find('\n', posFelicidadNum)) - posFelicidadNum,
			std::to_string(data->numFelicidad) + ",");
		int posMisionesAc = contenido.find("numMisionesAceptadas", posPersonaje) + 23;
		contenido.replace(posMisionesAc, (contenido.find('\n', posMisionesAc)) - posMisionesAc,
			std::to_string(data->numMisionesAceptadas) + ",");
	}

	// Abrir el archivo en modo de escritura
	std::ofstream archivoSalida("recursos/data/npcData.json");

	if (!archivoSalida.is_open()) {
		std::cout << "Error al abrir el archivo npcData.json para escritura." << std::endl;
		throw std::runtime_error("Error al escribir npcData.json");
	}
	
	// Escribir el contenido modificado en el archivo
	archivoSalida.clear();
	archivoSalida << contenido;
	archivoSalida.close();
}

void GeneralData::saveGame() {
	std::ifstream archivo("recursos/data/saveFile.json");

	if (!archivo.is_open())
	{
		std::cout << "Error al abrir saveFile.json" << std::endl;
		throw std::runtime_error("Error al abrir saveFile.json");
	}

	// Leer el contenido del archivo en una cadena
	std::string contenido = "";
	std::string linea;
	while (std::getline(archivo, linea)) {
		contenido += linea + "\n";
	}
	archivo.close();

	// cambiar el dia
	int posDia = contenido.find("Dia") + 6;
	size_t finLinea = contenido.find('\n', posDia);
	contenido.replace(posDia, finLinea - posDia, std::to_string(generalData().getDay()) + ",");

	//cambiar el dinero

	int posDinero = contenido.find("Dinero") + 9;
	finLinea = contenido.find('\n', posDinero);
	contenido.replace(posDinero, finLinea - posDinero, std::to_string(generalData().getMoney()));

	// Abrir el archivo en modo de escritura
	std::ofstream archivoSalida("recursos/data/saveFile.json");

	if (!archivoSalida.is_open()) {
		std::cout << "Error al abrir el archivo saveFile.json para escritura." << std::endl;
		throw std::runtime_error("Error al escribir saveFile.json");
	}

	// Escribir el contenido modificado en el archivo
	archivoSalida.clear();
	archivoSalida << contenido;
	archivoSalida.close();

	writeNPCData();
}

void GeneralData::incrementarFelicidad(Personaje p, int felicidadIncr)
{
	int actualFelicidad = getNPCData(p)->numFelicidad;
	int newFelicidadInt = actualFelicidad + felicidadIncr;
	if (newFelicidadInt < 0)
		newFelicidadInt = 0;
	else if (newFelicidadInt > 100)
		newFelicidadInt = 100;

	Felicidad newFelicidad = (Felicidad)newFelicidadInt;
	

	getNPCData(p)->felicidad = newFelicidad;
	getNPCData(p)->numFelicidad = newFelicidadInt;
}

void GeneralData::unlockMejoraPersonaje(Personaje p) {

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
	Felicidad aux = Normal;
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

//void GeneralData::setDayData() {
//	for (int i = 0; i < 7; i++)
//	{
//		npcData.at((Personaje)i)->setupDayData();
//	}
//}
std::string GeneralData::felicidadToString(Felicidad f)
{
	std::string aux = "";
	if (f == Minima)
		aux = "Minima";
	else if (f == Mala)
		aux = "Mala";
	else if (f == Normal)
		aux = "Normal";
	else if (f == Buena)
		aux = "Buena";
	else if (f == Maxima)
		aux = "Maxima";
	else if (f == NoHabladoAun)
		aux = "NoHabladoAun";
	return aux;
}

const std::string GeneralData::calleToString(Calle calle) {
	std::string aux = "";
	switch (calle) {
	case C1:
		aux = "C1";
		break;
	case C2:
		aux = "C2";
		break;
	case C3:
		aux = "C3";
		break;
	case Erronea:
		aux = "Erronea";
		break;
	}
	return aux;
}

Calle GeneralData::stringToCalle(const std::string& calle) {
	Calle aux = Erronea;
	if (calle == "C1")
		aux = C1;
	else if (calle == "C2")
		aux = C2;
	else if (calle == "C3")
		aux = C3;
	else if (calle == "Erronea")
		aux = Erronea;
	return aux;
}

const std::string GeneralData::tipoPaqueteToString(TipoPaquete tipo) {
	std::string aux = "";
	if (tipo == Alimento)
		aux = "Alimento";
	else if (tipo == Medicinas)
		aux = "Medicinas";
	else if (tipo == Joyas)
		aux = "Joyas";
	else if (tipo == Materiales)
		aux = "Materiales";
	else if (tipo == Armamento)
		aux = "Armamento";
	return aux;
}

TipoPaquete GeneralData::stringToTipoPaquete(const std::string& tipo) {
	TipoPaquete aux = Alimento;
	if (tipo == "Alimento")
		aux = Alimento;
	else if (tipo == "Medicinas")
		aux = Medicinas;
	else if (tipo == "Joyas")
		aux = Joyas;
	else if (tipo == "Materiales")
		aux = Materiales;
	else if (tipo == "Armamento")
		aux = Armamento;
	return aux;
}

const std::string GeneralData::nivelPesoToString(NivelPeso nivel)
{
	std::string aux = "";
	switch (nivel) {
	case NivelPeso::Bajo:
		aux = "Bajo";
	case NivelPeso::Medio:
		aux = "Medio";
	case NivelPeso::Alto:
		aux = "Alto";
	case NivelPeso::Ninguno:
		aux = "Ninguno";
	}
	return aux;
}

NivelPeso GeneralData::stringToNivelPeso(const std::string& nivel)
{
	NivelPeso aux = Ninguno;
	if (nivel == "Bajo")
		aux = Bajo;
	else if (nivel == "Medio")
		aux = Medio;
	else if (nivel == "Alto")
		aux = Alto;
	else if (nivel == "Ninguno")
		aux = Ninguno;
	return aux;
}

// Struct NPCdata
#pragma region NPCdata

NPCdata* GeneralData::getNPCData(Personaje personaje) {
	NPCdata* npc = nullptr;

	npc = npcData[personaje];

	return npc;
}

#pragma endregion