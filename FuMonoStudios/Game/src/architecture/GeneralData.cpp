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
	for (int i = 0; i < 7; i++)
	{
		npcDataVec_[i]->setupDayData();
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
			NPCMayorData* data = new NPCMayorData(stringToFelicidad(felicidadStr));
			data->numMisionesAceptadas = jObject.find("numMisionesAceptadas")->second->AsNumber();
			data->numFelicidad = jObject.find("FelicidadNum")->second->AsNumber();
			npcDataVec_.push_back(data);
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
			NPCMenorData* data = new NPCMenorData(stringToFelicidad(felicidadStr), diasDanEventos);
			data->numMisionesAceptadas = jObject.find("numMisionesAceptadas")->second->AsNumber();
			data->numFelicidad = jObject.find("FelicidadNum")->second->AsNumber();
			npcDataVec_.push_back(data);
		}
		jValueRoot = nullptr;
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
		case Personaje::Vagabundo:
			aux = "Vagabundo";
			break;
		case Personaje::Secretario:
			aux = "Secretario";
			break;
		case Personaje::Campesino:
			aux = "Campesino";
			break;
		case Personaje::Artesano:
			aux = "Artesano";
			break;
		case Personaje::Tarotisa:
			aux = "Tarotisa";
			break;
		case Personaje::Soldado:
			aux = "Soldado";
			break;
		case Personaje::Contable:
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
		aux = Personaje::Vagabundo;
	else if (pers == "Secretario")
		aux = Personaje::Secretario;
	else if (pers == "Campesino")
		aux = Personaje::Campesino;
	else if (pers == "Artesano")
		aux = Personaje::Artesano;
	else if (pers == "Tarotisa")
		aux = Personaje::Tarotisa;
	else if (pers == "Soldado")
		aux = Personaje::Soldado;
	else if (pers == "Contable")
		aux = Personaje::Contable;
	
	return aux;
}

Felicidad GeneralData::stringToFelicidad(const std::string& str)
{
	Felicidad aux = Felicidad::Normal;
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

std::string GeneralData::felicidadToString(Felicidad f)
{
	std::string aux = "";
	if (f == Felicidad::Minima)
		aux = "Minima";
	else if (f == Felicidad::Mala)
		aux = "Mala";
	else if (f == Felicidad::Normal)
		aux = "Normal";
	else if (f == Felicidad::Buena)
		aux = "Buena";
	else if (f == Felicidad::Maxima)
		aux = "Maxima";
	else if (f == Felicidad::NoHabladoAun)
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
}
void GeneralData::setDayData() {
	for (int i = 0; i < 7; i++)
	{
		npcData.at((Personaje)i)->setupDayData();
	}
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

	if (felicidad == Felicidad::Minima || felicidad == Felicidad::Maxima || felicidad == Felicidad::NoHabladoAun)
	{
		switch (felicidad)
		{
		case Felicidad::NoHabladoAun:
			tipo = "Presentacion";
			felicidad = Felicidad::Normal;
			break;
		case Felicidad::Minima:
			tipo = "FelicidadMinimo";
			break;
		case Felicidad::Maxima:
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
			case Felicidad::Mala:
				tipo = "GenericoMalo";
				iterateDialogues();
				iterationNum = iteration;
				break;
			case Felicidad::Normal:
				tipo = "GenericoNormal";
				iterateDialogues();
				iterationNum = iteration;
				break;
			case Felicidad::Buena:
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

NPCevent* GeneralData::NPCMenorData::getEvent() {
	return events[sdlutils().rand().nextInt(0,5)];
	numMisionesAceptadas++;
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
	case Felicidad::NoHabladoAun:
			aux = "Presentacion";
			felicidad = Felicidad::Normal;
			break;
		case Felicidad::Minima:
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

NPCevent* GeneralData::NPCMayorData::getEvent() {
	return events[numMisionesAceptadas];
	numMisionesAceptadas++;
}
#pragma endregion