#pragma once
#include "../utils/Singleton.h"
#include "../sistemas/Felicidad.h"
#include "GameConstants.h"
#include <architecture/ecs.h>
#include <vector>
#include <string>
#include <iostream>

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
	enum TipoPaquete { Alimento, Medicinas, Joyas, Materiales, Armamento };
	/// <summary>
	/// enum con todas los tipos de medici�n de peso que pueden tener los paquetes
	/// </summary>
	enum NivelPeso { Ninguno, Bajo, Medio, Alto };
}

using namespace pq;
class GeneralData : public Singleton<GeneralData>
{
public:
	friend Singleton<GeneralData>;

	// enum con tipos de felicidad
	enum Felicidad { Minima, Mala, Normal, Buena, Maxima, NoHabladoAun };
	
	Felicidad stringToFelicidad(const std::string& str);

	// enum con el nombre de todos los NPC
	enum Personaje {
		Vagabundo, Secretario, Campesino, Artesano, Tarotisa, Soldado, Contable
	};

	// enum con el nombre de todos los Objetos Interactuables
	enum InteractableObj {
		Casa1, Casa2
	};

	enum MoveType{DropIn, PickUp};


	#pragma region NPCdata

	// Los datos de los NPC deben actualizarse al acabar cada día.
	// Recogen datos sobre su felicidad, así como que dialogo deben enseñar.
	// Al iniciarse, su felicidad estará en NoHabladoAun, y al sacar su
	// primer diálogo cambiará a Normal.
	// NPC MENORES: El bool giveEvent dicta si debe dar evento (true) o dar
	// un dialogo generico (false). El int iteration itera sobre los 3 posibles
	// dialogos genericos que tiene el personaje.
	// NPC GRANDES: El bool postConversation si es true, significa que ya se 
	// ha hablado con el una vez, y sacara el dialogo mas corto que sale despues
	// del dialogo original de ese dia.
	// 
	// Al acabar el día se debe llamar a setupDayData() para reiniciar las 
	// variables y ajustar datos segun el dia
	// 
	// MIRAR EL comoEscribirEventos.MD PARA SABER COMO USAR ESTO
	
	struct IntObjsData {

		virtual std::pair<const std::string, int> getDialogueInfo();

	};

	struct NPCdata {
		Felicidad felicidad;
		
		int numMisionesAceptadas;
		std::vector<NPCevent*> events;
		virtual NPCevent* getEvent() = 0;

		virtual std::pair<const std::string, int> getDialogueInfo() = 0;

		// esto solo lo usa el NPCmenor
		virtual void iterateDialogues() = 0;
		virtual void setupDayData() = 0;
	};

	struct NPCMenorData : public NPCdata {
		NPCMenorData(Felicidad Felicidad, std::vector<bool> DiasDanEvento);

		NPCevent* getEvent() override;

		std::pair<const std::string, int> getDialogueInfo() override;
		void iterateDialogues() override;
		void setupDayData() override;
	private:
		void activateEvent();
		void deactivateEvent();

		std::vector<bool> diasDanEvento;

		bool giveEvent;
		int iteration;
	};

	struct NPCMayorData : public NPCdata {
		NPCMayorData(Felicidad Felicidad);

		NPCevent* getEvent() override;

		std::pair<const std::string, int> getDialogueInfo() override;
		void iterateDialogues() override {};
		void setupDayData() override;
	private:
		bool postConversation;
	};
	
	// METODOS DE NPCdata

	void readNPCData();
	void writeNPCData();

	NPCdata* getNPCData(Personaje personaje);

	void incrementarFelicidad(Personaje p, int felicidadIncr);

	NPCeventSystem* npcEventSys = nullptr;

	// METODOS DE INTOBJSdata

	//void readIntObjData();

	IntObjsData* getObjData(InteractableObj intobj);

private:
	// vector que contiene los datos de todos los 7 npc
	std::vector<NPCdata*> npcData;
	std::vector<InteractableObj*> intObjData;
#pragma endregion
public:
	GeneralData();
	~GeneralData();

	/// <summary>
	/// Metodo que acutaliza cuanto dinero tienes en funcion de los fallos y aciertos que realices
	/// </summary>
	/// <param name="writePacages"></param>
	/// <param name="wrongPacages"></param>
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

	const std::string personajeToString(Personaje pers);
	Personaje stringToPersonaje(const std::string& pers);


	const std::string objetoToString(InteractableObj pers);
	InteractableObj stringToObj(const std::string& obj);

	std::string fromDistritoToString(int i);
	int fromStringToDistrito(std::string place);
	const std::string calleToString(Calle calle);
	Calle stringToCalle(const std::string& calle);
	const std::string tipoPaqueteToString(TipoPaquete tipo);
	TipoPaquete stringToTipoPaquete(const std::string& tipo);
	const std::string nivelPesoToString(NivelPeso nivel);
	NivelPeso stringToNivelPeso(const std::string& nivel);

	void updateMoney();
	//Los métodos para acceder a las herramientas que te pueden dar los NPCs
	void aquireSelloMulticolor() { selloMulticolor = true; }
	bool getSelloMulticolor() { return selloMulticolor; }

	void unlockMejoraPersonaje(Personaje p);
private:
	void addMoney(int cant) { dinero_ += cant; }
	void reduceMoney(int cant) { dinero_ -= cant; }

	void updateDistrictsPerDay(int dia);

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
	std::vector<std::string> placesToActive_;
	
	std::vector<bool> upgrades_;
	//Aqui van las variables que indican si se han conseguido las herramientas especiales de los NPCs
	bool selloMulticolor = false; //Sello multicolor debe estar debtro de updates
};

inline GeneralData& generalData() {
	return *GeneralData::instance();
}