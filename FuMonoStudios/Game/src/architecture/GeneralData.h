#pragma once
#include <utils/checkML.h>
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

	enum MoveType{DropIn, PickUp};

	/// <summary>
	/// Estructura que almacena la informacion de los objetos interactuables
	/// </summary>
	struct IntObjsData {
		IntObjsData(std::string text);
		std::string objId;
	};
	/// <summary>
	/// Carga los datos de los npc es decir: Felicidad, Misiones pendientes, Eventos
	/// </summary>
	void readNPCData();
	/// <summary>
	/// Guarda los datos de los npc es decir: Felicidad, Misiones pendientes, Eventos
	/// </summary>
	void writeNPCData();
	/// <summary>
	/// Devuelve los datos del npc solicitado
	/// </summary>
	/// <param name="personaje"></param>
	/// <returns></returns>
	NPCdata* getNPCData(Personaje personaje);
	/// <summary>
	/// Incrementa la felicidad del personaje indicado
	/// </summary>
	/// <param name="p"></param>
	/// <param name="felicidadIncr"></param>
	void incrementarFelicidad(Personaje p, int felicidadIncr);

	NPCeventSystem* npcEventSys = nullptr;

private:
	/// <summary>
	/// vector que contiene los datos de todos los objetos interactuables
	/// </summary>
	std::vector<IntObjsData*> intObjData;
#pragma endregion
public:
	GeneralData();
	~GeneralData();
	/// <summary>
	/// Caraga la partida guardada
	/// </summary>
	void loadSaveFile();
	/// <summary>
	/// Metodo que acutaliza cuanto dinero tienes en funcion de los fallos y aciertos que realices
	/// </summary>
	/// <param name="writePacages"></param>
	/// <param name="wrongPacages"></param>
	void updateMoney();

	/// <summary>
	/// Calcula el dinero que recibe el jugador en base a los aciertos y fallos.
	/// </summary>
	/// <returns></returns>
	int calcularDineroGanado();
	
	/// <summary>
	/// Reestablece el dinero a la cantidad inicial
	/// </summary>
	void resetMoney(); 

	/// <summary>
	/// Devuelve la cantidad de dinero que tiene el jugador
	/// </summary>
	/// <returns></returns>
	inline int getMoney() { return dinero_; }

	inline void setUpgradeValue(ecs::upg::upgradeId upgrade, bool value) {
		upgrades_[upgrade] = value;
	}

	inline bool getUpgradeValue(ecs::upg::upgradeId upgrade) {
		return upgrades_[upgrade];
	}

	inline void unlockUpgrade(npc::Personaje pers) {
		switch (pers) {
		case Vagabundo: 
			setUpgradeValue(ecs::upg::SELLO_UPGRADE, true);
			break;
		case Tarotisa:
			setUpgradeValue(ecs::upg::BOLA_UPGRADE, true);
			break;
		case Artesano:
			setUpgradeValue(ecs::upg::ENVOLVER_UPGRADE, true);
			break;
		case Campesino:
			setUpgradeValue(ecs::upg::BALANZA_UPGRADE, true);
			break;
		case Contable:
			setUpgradeValue(ecs::upg::MONEY_UPGRADE, true);
			break;
		case Soldado:
			setUpgradeValue(ecs::upg::FALLOS_UPGRADE, true);
			break;
		case Secretario:
			setUpgradeValue(ecs::upg::MANUAL_UPGRADE, true);
			break;
		}
	}

	void setFinalID(int final); //Cambia el ID del final
	int getFinalID(); //Devuelve el id del final del juego

	void changeParamID(int i, bool suma); //Modifica un parametro en especifico del array y decide si se suma o no
	inline int getParam(int i) {
#ifdef _DEBUG
		std::cout << "El valor del parametro que quieres es: " << paramAjustes_[i] << std::endl;
#endif // _DEBUG

		return paramAjustes_[i]; 
	}

	inline int getDay() { return dia_; }
	inline void setDay(int dia) { dia_ = dia; updateDia(); }

	inline int getNumDistritos() { return (Distrito::Erroneo); }


	void updateDia();

	inline std::vector<std::string> getPlacesToActive() { return placesToActive_; }

	inline void setTubesAmount(int tubos) { 
		if (tubos >= 7) numTubos_ = 7;
		else numTubos_ = tubos; 
	} // Aumenta el numero de tubos en el minijuego cuando se requiera (podría llamarse automáticamente
	// desde setDia modificado). Que jose lo haga cuando se sepan los días en los que un distrito y su tubo se desbloquean
	inline int getTubesAmount() { return numTubos_; }
	inline void correctPackage() { corrects_++; }
	inline void wrongPackage() { fails_++; }
	inline int getFails() { return fails_; }
	inline int getCorrects() { return corrects_; }

	inline void resetFailsCorrects() { fails_ = 0; corrects_ = 0; }

	int getPaqueteLevel(); // Devuelve el lvl del paquete correspondiente al d�a
	void setPaqueteLevel(int lvl);

	int getRent();
	void setRent(int rent);

	//Textos personajes
	const std::string personajeToString(Personaje pers);
	Personaje stringToPersonaje(const std::string& pers);

	//Textos objetos
	const std::string intObjetoToString(int pers);
	int stringToObjInt(const std::string& obj);

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

	void unlockMejoraPersonaje(Personaje p);
	/// <summary>
	/// Guarda el juego
	/// </summary>
	void saveGame();

	void newGame();
private:
	void addMoney(int cant) { dinero_ += cant; }
	void reduceMoney(int cant) { dinero_ -= cant; }

	void updateDistrictsPerDay(int dia);
	/// <summary>
	/// estructura que almacena los datos de los npc
	/// </summary>
	std::unordered_map<Personaje,NPCdata*> npcData;
	/// <summary>
	/// Fallos cometidos por el jugador
	/// </summary>
	int fails_;
	/// <summary>
	/// Aciertos conseguidos por el jugador
	/// </summary>
	int corrects_;
	/// <summary>
	/// Precio del alquiler
	/// </summary>
	int rent_;
	/// <summary>
	/// Dinero que tiene le jugador
	/// </summary>
	int dinero_;
	/// <summary>
	/// Margen de fallos que puede cometer el jugador
	/// </summary>
	int failsMargin_;

	int finalID_; //Variable int que define en la �ltima escena cu�l final se va a reproducir
	/// <summary>
	/// Dia de juego en el que nos encontramos
	/// </summary>
	int dia_;
	/// <summary>
	/// Clasificacion de distribucion de la creacion de paquetes
	/// </summary>
	int paqueteLvl_ = 0;

	/// <summary>
	/// Array de ints en el que cada posicion corresponde al numero de configuracion de un parametro
	/// dentro de los ajustes del juego. Por ejemplo, la posición 1 puede corresponder 
	/// nivel de volumen (maximo 100).
	/// </summary>
	int paramAjustes_[10];

	// Si en verdad en cuanto desbloqueas un distrito que explorar, aparece el tubo correspondiente en la oficina,
	// podemos hacer que la variable de numero de tubos y del numero de distritos desbloqueados sean una sola para simplificar todo
	
	/// <summary>
	/// Numero de tubos que habrán en el minijuego de paquetes
	/// </summary>
	int numTubos_;
	/// <summary>
	/// Luis: no se que hace realmente este vector si alguien sabe especificar que lo ponga porfavor y gracias
	/// </summary>
	std::vector<Paquete*> paquetesNPCs;
	/// <summary>
	/// Luis: no se que hace realmente este vector si alguien sabe especificar que lo ponga porfavor y gracias
	/// </summary>
	std::vector<std::string> placesToActive_;
	/// <summary>
	/// Vector con las mejoras desbloqueadas hasta el momento
	/// </summary>
	std::vector<bool> upgrades_;	

};

inline GeneralData& generalData() {
	return *GeneralData::instance();
}