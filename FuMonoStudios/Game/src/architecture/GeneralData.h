#pragma once
#include "../utils/Singleton.h"
#include "GameConstants.h"
#include <fstream>
#include <sistemas/NPC.h>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

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

namespace npc {

	// enum con tipos de felicidad
	enum Felicidad { Minima, Mala, Normal, Buena, Maxima, NoHabladoAun };

	// enum con el nombre de todos los NPC
	enum Personaje {
		Vagabundo, Secretario, Campesino, Artesano, Tarotisa, Soldado, Contable
	};

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
	
	struct NPCdata {
		Felicidad felicidad;
		int numFelicidad;
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

	void setDayData();
	

	NPCdata* getNPCData(Personaje personaje);

	void incrementarFelicidad(Personaje p, int felicidadIncr);

	NPCeventSystem* npcEventSys = nullptr;
private:
	// vector que contiene los datos de todos los 7 npc
	std::vector<NPCdata*> npcDataVec_;
#pragma endregion
public:
	GeneralData();
	~GeneralData();

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

	void setFinalID(int final); //Cambia el ID del final
	int getFinalID(); //Devuelve el id del final del juego


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

	int getCharacterEventID(int p) {
		return charactersEvents_[p];
	}

	void setCharacterEventID(int p, int e) {
		charactersEvents_[p] = e;
	}

	void resetFailsCorrects() { fails_ = 0; corrects_ = 0; }

	int getPaqueteLevel(); // Devuelve el lvl del paquete correspondiente al d�a
	void setPaqueteLevel(int lvl);

	int getRent();
	void setRent(int rent);

	const std::string personajeToString(Personaje pers);
	Personaje stringToPersonaje(const std::string& pers);
	std::string fromDistritoToString(int i);
	int fromStringToDistrito(std::string place);
	const std::string calleToString(Calle calle);
	Calle stringToCalle(const std::string& calle);
	const std::string tipoPaqueteToString(TipoPaquete tipo);
	TipoPaquete stringToTipoPaquete(const std::string& tipo);
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
	// Si en verdad en cuanto desbloqueas un distrito que explorar, aparece el tubo correspondiente en la oficina,
	// podemos hacer que la variable de numero de tubos y del numero de distritos desbloqueados sean una sola para simplificar todo
	int numTubos_; // Numero de tubos que habrán en el minijuego de paquetes
	int charactersEvents_[7]; // Recoge los eventos de paquete de cada personaje
	std::vector<Paquete*> paquetesNPCs;
	std::vector<std::string> placesToActive_;

	//Aqui van las variables que indican si se han conseguido las herramientas especiales de los NPCs
	bool selloMulticolor = false;

};

inline GeneralData& generalData() {
	return *GeneralData::instance();
}