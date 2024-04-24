#pragma once

#include "../architecture/Component.h"
#include "../architecture/GeneralData.h"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../utils/Vector2D.h"
#include "Transform.h"


class Scene;

class Paquete : public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::PAQUETE)
		

	Paquete(Paquete&);
	Paquete(pq::Distrito, pq::Calle, const std::string& nombreCalle,const std::string& remitente, TipoPaquete, bool correcto = true, NivelPeso nivPeso = Ninguno, int peso = 0,
		bool fragil = false, bool carta = false);
	~Paquete();

	void initComponent() override;
	bool correcto() const;		//Bool que comprueba si el paquete tiene errores (falsificaciones, calles incorrectas...)
	bool bienSellado() const;

	// Sella la calle una única vez con el sellador
	void sellarCalle(pq::Calle sello, Transform* trSellador, bool);

	// envuelve el paquete
	void envolver() { envuelto_ = true; }

	//Genera los puntos rojos al envolver
	void puntosRojos(int routeID);

	//Genera las linea rojas entre los puntos rojos
	void drawLines(int routeID, std::string routeName);
	//Limpia las entidades de una layer que le pases
	void clearLayer(ecs::layer::layerId lyId);

	/// <summary>
	/// metodo que devuelve el string a implimir en la etiqueta de direccion
	/// </summary>
	/// <returns></returns>
	std::string getDirecction();
	pq::TipoPaquete getTipo() const { return miTipo_; }
	pq::Distrito getDistrito() const { return miDistrito_; }
	pq::Calle getCalle() const { return miCalle_; }
	pq::Calle getCalleMarcada() const { return calleMarcada_; }
	std::string getRemitente() const { return miRemitente_; }
	NivelPeso getPeso() const { return miPeso_; }
	int getCantidadPeso() const { return peso_; }
	bool getFragil() const { return fragil_; }
	bool getEnvuelto() const { return envuelto_; }
	bool getSelloCorrecto() const { return selloCorrecto_; }
	bool isCarta() const { return carta_; }
	bool pesoCorrecto() const;
	bool correctFragile() const;
	void giveData(std::ofstream&) const;

private:
	//void createSello(std::string texKey, int x, int y, int width, int height); (movido al paqueteBuilder)

	//Variables que se generan automaticamente con informaci�n de los paquetes
	pq::Distrito miDistrito_;	//Variable con el distrito al que es enviado el paquete	
	std::string miRemitente_;  //Variable con el nombre del remitente
	pq::Calle miCalle_;			//Variable con la calle a la que es enviada el paquete	
	std::string nombreCalle_;
	TipoPaquete miTipo_;		//Variable con el tipo de cargamente que lleva el paquete
	bool selloCorrecto_;		//Variable que indica si el sello que contiene el paquete es correcto o no
	NivelPeso miPeso_;		//Variable que indica qu� peso esta marcado en el paquete, o si este peso ni siquera est� marcado
	int peso_;				//Variable que indica cuanto pesa el paquete para mostrar en la balanza
	bool fragil_;			//Variable que indica si tiene un sello de fr�gil puesto

	//Bool que indica si el paquete es en realidad una carta, y si debe ser representado como tal en el renderizado
	bool carta_;

	//Variables que debe modificar el jugador
	pq::Calle calleMarcada_;		//Variable que indica para qu� distrito ha sido etiquetado el paquete
	bool envuelto_;			//Variable que indica si est� envuelto o no el paquete
	int routeID; //ID de la ruta de la caja creada

	Scene* scene_;
};

