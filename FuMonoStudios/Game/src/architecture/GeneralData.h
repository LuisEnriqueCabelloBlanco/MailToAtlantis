#pragma once
#include "../utils/Singleton.h"
#include "GameConstants.h"
#include <architecture/ecs.h>
#include <sistemas/NPC.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>

class DialogManager;
class PaqueteBuilder;
class Game;
class Paquete;

class NPCevent;
class NPCeventSystem;

namespace pq {
	/// <summary>
	/// enum con todos los distritos posibles que pueden tener los paquetes
	/// IMPORTANTE: Erroneo siempre debe ser el ultimo
	/// </summary>
	enum Distrito { Hestia, Artemisa, Demeter, Hefesto, Hermes, Apolo, Poseidon, Erroneo };
	/// <summary>
	/// enum con todas las calles posibles que pueden tener los paquetes
	/// </summary>
	enum Calle { C1, C2, C3, Erronea };
	/// <summary>
	/// enum con todoos los tipos de cargamento que pueden tener los paquetes
	/// </summary>
	enum TipoPaquete { Alimento, Medicinas, Joyas, Materiales, Armamento, Carta };
	/// <summary>
	/// enum con todas los tipos de medici�n de peso que pueden tener los paquetes
	/// </summary>
	enum NivelPeso { Ninguno, Bajo, Medio, Alto };
}

using namespace pq;
using namespace npc;
class GeneralData : public Singleton<GeneralData>
{
public:
	friend Singleton<GeneralData>;
	
	Felicidad stringToFelicidad(const std::string& str);
	std::string felicidadToString(Felicidad);

	// enum con el nombre de todos los Objetos Interactuables
	enum InteractableObj {
		CasaGrande, CartelOficina, Muro, //Hestia
		TiendaPociones, TiendaBolas, TiendaJarrones, //Artemisa
		Molino, Arbol, Carreta, //Demeter
		PulpoCartel, TiendaCeramica, TiendaEsculturas, //Hefesto
		TiendaDerecha, PanteonIzq, PanteonDer, //Hermes
		Panteon, Edificios, Charco, //Apolo
		Casa1, Casa2 //Poseidon
	};

	enum MoveType{DropIn, PickUp};

	
	struct IntObjsData {
		IntObjsData(InteractableObj text);
		InteractableObj texto;
		virtual const std::string getDialogueInfo();

	};
#pragma endregion

	void readNPCData();
	void writeNPCData();

	NPCdata* getNPCData(Personaje personaje);

	void incrementarFelicidad(Personaje p, int felicidadIncr);

	NPCeventSystem* npcEventSys = nullptr;

	// METODOS DE INTOBJSdata

	void readIntObjData();

	IntObjsData* getObjData(InteractableObj intobj);

private:
	// vector que contiene los datos de todos los objetos interactuables
	std::vector<IntObjsData*> intObjData;
	// vector que contiene los datos de todos los 7 npc
	std::vector<NPCdata*> npcDataVec_;
#pragma endregion
public:
	GeneralData();
	~GeneralData();

	void loadSaveFile();
	/// <summary>
	/// Metodo que acutaliza cuanto dinero tienes en funcion de los fallos y aciertos que realices
	/// </summary>
	/// <param name="writePacages"></param>
	/// <param name="wrongPacages"></param>
	void updateMoney();

	//Calcula el dinero que recibe el jugador en base a los aciertos y fallos.
	//Ejemplos de uso son en el metodo de arriba o en la endWorkScene
	int calcularDineroGanado();
	
	void resetMoney(); //Pone el dinero a INITIAL_MONEY

	int getMoney() { return dinero_; }

	void setUpgradeValue(ecs::upg::upgradeId upgrade, bool value) {
		upgrades_[upgrade] = value;
	}

	bool getUpgradeValue(ecs::upg::upgradeId upgrade) {
		return upgrades_[upgrade];
	}

	void setFinalID(int final); //Cambia el ID del final
	int getFinalID(); //Devuelve el id del final del juego

	void changeParamID(int i, bool suma); //Modifica un parametro en especifico del array y decide si se suma o no
	int getParam(int i) {
		std::cout << "El valor del parametro que quieres es: " << paramAjustes_[i] << std::endl;
		return paramAjustes_[i]; 
	}

	int getDay() { return dia_; }
	void setDay(int dia) { dia_ = dia; updateDia(); }

	int getNumDistritos() { return (Distrito::Erroneo); }


	void updateDia();

	std::vector<std::string> getPlacesToActive() { return placesToActive_; }

	void setTubesAmount(int tubos) { 
		if (tubos >= 7) numTubos_ = 7;
		else numTubos_ = tubos; 
	} // Aumenta el numero de tubos en el minijuego cuando se requiera (podría llamarse automáticamente
	// desde setDia modificado). Que jose lo haga cuando se sepan los días en los que un distrito y su tubo se desbloquean
	int getTubesAmount() { return numTubos_; }
	void correctPackage() { corrects_++; }
	void wrongPackage() { fails_++; }
	int getFails() { return fails_; }
	int getCorrects() { return corrects_; }

	void resetFailsCorrects() { fails_ = 0; corrects_ = 0; }

	int getPaqueteLevel(); // Devuelve el lvl del paquete correspondiente al d�a
	void setPaqueteLevel(int lvl);

	int getRent();
	void setRent(int rent);

	//Textos personajes
	const std::string personajeToString(Personaje pers);
	Personaje stringToPersonaje(const std::string& pers);

	//Textos objetos
	const std::string objetoToString(InteractableObj pers);
	InteractableObj stringToObj(const std::string& obj);

	//Textos distritos
	std::string fromDistritoToString(int i);
	int fromStringToDistrito(std::string place);

	//Textos calles
	const std::string calleToString(Calle calle);
	Calle stringToCalle(const std::string& calle);

	//Textos paquetes
	const std::string tipoPaqueteToString(TipoPaquete tipo);
	TipoPaquete stringToTipoPaquete(const std::string& tipo);

	//Textos peso
	const std::string nivelPesoToString(NivelPeso nivel);
	NivelPeso stringToNivelPeso(const std::string& nivel);

	//Los métodos para acceder a las herramientas que te pueden dar los NPCs
	void aquireSelloMulticolor() { selloMulticolor = true; }
	bool getSelloMulticolor() { return selloMulticolor; }

	void unlockMejoraPersonaje(Personaje p);

	void saveGame();
private:
	void addMoney(int cant) { dinero_ += cant; }
	void reduceMoney(int cant) { dinero_ -= cant; }

	void updateDistrictsPerDay(int dia);
	// vector que contiene los datos de todos los 7 npc
	std::unordered_map<Personaje,NPCdata*> npcData;

	int fails_;
	int corrects_;
	int rent_;
	int dinero_;
	int failsMargin_;
	int finalID_; //Variable int que define en la �ltima escena cu�l final se va a reproducir
	int dia_;
	int paqueteLvl_ = 0; // de momento es 0

	int paramAjustes_[10]; //Array de ints en el que cada posicion corresponde al numero de configuracion de un parametro
						  //dentro de los ajustes del juego. Por ejemplo, la posición 1 puede corresponder al
	                      //nivel de volumen (maximo 100).

	// Si en verdad en cuanto desbloqueas un distrito que explorar, aparece el tubo correspondiente en la oficina,
	// podemos hacer que la variable de numero de tubos y del numero de distritos desbloqueados sean una sola para simplificar todo
	int numTubos_; // Numero de tubos que habrán en el minijuego de paquetes
	std::vector<Paquete*> paquetesNPCs;
	std::vector<std::string> placesToActive_;
	
	std::vector<bool> upgrades_;
	//Aqui van las variables que indican si se han conseguido las herramientas especiales de los NPCs
	bool selloMulticolor = false; //Sello multicolor debe estar debtro de updates
};

inline GeneralData& generalData() {
	return *GeneralData::instance();
}