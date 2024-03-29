#pragma once
#include "../utils/Singleton.h"
#include "../components/Paquete.h"
#include "../sistemas/Felicidad.h"
#include "GameConstants.h"
#include <vector>

struct DatosPersonajes {
	pers::Personajes p;
	pers::EstadosDeFelicidad f;
};

class GeneralData : public Singleton<GeneralData>
{
	friend Singleton<GeneralData>;
public:
	GeneralData() :dinero_(INITIAL_MONEY), finalID_(INITIAL_FINAL), eventoID_(INITIAL_EVENT),failsMargin_(INITIAL_FAILS_MARGIN),
		corrects_(0), fails_(0), dia_(1), numTubos_(INITIAL_TUBE_AMOUNT) {
		updateFelicidadPersonajes();
	};
	~GeneralData(){};

	/// <summary>
	/// Metodo que acutaliza cuanto dinero tienes en funcion de los fallos y aciertos que realices
	/// </summary>
	/// <param name="writePacages"></param>
	/// <param name="wrongPacages"></param>
	void updateMoney(int correct, int wrong);
	int getMoney() { return dinero_; }

	void setFinalID(int final); //Cambia el ID del final
	int getFinalID(); //Devuelve el id del final del juego

	void setEventoID(int evento); //Cambia el ID del evento a ocurrir
	int getEventoID(); //Devuelve el id del evento que ocurrir� en el juego

	int getDia() { return dia_; }
	void setDia(int dia) { dia_ = dia; }

	void setTubesAmount(int tubos) { 
		if (tubos >= 7) numTubos_ = 7;
		else numTubos_ = tubos; 
	} // Aumenta el numero de tubos en el minijuego cuando se requiera (podría llamarse automáticamente
														  // desde setDia modificado). Que jose lo haga cuando se sepan los días en los que un distrito y su tubo se desbloquean
	int getTubesAmount() { return numTubos_; }

	// Aumenta el numero de distritos desbloqueados
	void setDistrictsAmount(int distritos) {
		if (distritos >= 7) numDistritos_ = 7;
		else numDistritos_ = distritos;
	}
	int getDistrictsAmount() { return numDistritos_; }

	void correctPackage() { corrects_++; }
	void wrongPackage() { fails_++; }

	int getFails() { return fails_; }
	int getCorrects() { return corrects_; }

	//Quien borre el metodo de abajo le castro
	void updateFelicidadPersonajes() {
		for (int i = 0; i < 7; i++) {
			charactersData_[i].p = felicidad().getPersonaje(i);
			charactersData_[i].f = felicidad().interpretaFel(charactersData_[i].p);
			std::cout << "El personaje es: " << charactersData_[i].p << std::endl;
			std::cout << "Y su felicidad es: " << charactersData_[i].f << std::endl;
		}
	}

	void resetFailsCorrects() { fails_ = 0; corrects_ = 0; }
	void addPaqueteNPC(Paquete* p) { paquetesNPCs.push_back(p); }
	bool areTherePaquetesNPC() { return paquetesNPCs.size() != 0; }
	void resetPaquetesNPC() { while (areTherePaquetesNPC()) paquetesNPCs.pop_back(); }
	Paquete* getPaqueteNPC() { Paquete* p = paquetesNPCs.back(); paquetesNPCs.pop_back(); return p; }
	int getPaqueteLevel(); // Devuelve el lvl del paquete correspondiente al d�a
	void setPaqueteLevel(int lvl);

	inline int getCurrentDay() { return dia_; };

private:
	void addMoney(int cant) { dinero_ += cant; }
	void reduceMoney(int cant) { dinero_ -= cant; }

	int fails_;
	int corrects_;
	int dinero_;
	int failsMargin_;
	int finalID_; //Variable int que define en la �ltima escena cu�l final se va a reproducir
	int eventoID_; //Variable int que define cual evento especial de la historia deber� de ejecutarse
	int dia_;
	int paqueteLvl_ = 0; // de momento es 0
	int numTubos_; // Numero de tubos que habrán en el minijuego de paquetes
	int numDistritos_; // Numero de distritos que habrán sido desbloqueados
	//Quien borre lo de abajo le castro
	DatosPersonajes charactersData_[7]; // Recoge la felicidad de cada personaje
	std::vector<Paquete*> paquetesNPCs;
};

inline GeneralData& generalData() {
	return *GeneralData::instance();
}