#pragma once

#include "../architecture/Component.h"
#include <unordered_map>
#include <string>
#include <vector>

class Paquete : public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::PAQUETE)
	enum Distrito {Demeter,Hefesto,Hestia,Artemisa,Hermes,Apolo,Poseidon,Erroneo};	//enum con todos los distritos posibles que pueden tener los paquetes
	enum Calle {Erronea, C1,C2,C3};											//enum con todas las calles posibles que pueden tener los paquetes
	enum TipoPaquete { Alimento,Medicinas,Joyas,Materiales,Armamento };		//enum con todoos los tipos de cargamente que pueden tener los paquetes
	enum NivelPeso { Ninguno, Bajo, Medio,Alto };							//enum con todas los tipos de medici�n de peso que pueden tener los paquetes



	Paquete(Distrito, Calle, TipoPaquete, bool, NivelPeso, int, bool, bool);
	~Paquete();
	bool Correcto() const;		//Bool que comprueba si el paquete tiene errores (falsificaciones, calles incorrectas...)

	Distrito getDist() const;
	void sellarCalle(Calle sello);
	std::string getDirecction();

private:

	std::unordered_map<Distrito, std::vector<std::string>> distrito_calle = {
		{Demeter,{"Calle 1", "Calle 2","Calle 3"}},
		{Hefesto,{"Calle 1", "Calle 2","Calle 3"}},
		{Hestia,{"Calle 1", "Calle 2","Calle 3"}},
		{Artemisa,{"Calle 1", "Calle 2","Calle 3"}},
		{Hermes,{"Calle 1", "Calle 2","Calle 3"}},
		{Apolo,{"Calle 1", "Calle 2","Calle 3"}},
		{Poseidon,{"Calle 1", "Calle 2","Calle 3"}}
	};

	//Variables que se generan automaticamente
	Distrito miDistrito;	//Variable con el distrito al que es enviado el paquete	
	Calle miCalle;			//Variable con la calle a la que es enviada el paquete	
	TipoPaquete miTipo;		//Variable con el tipo de cargamente que lleva el paquete
	bool selloCorrecto;		//Variable que indica si el sello que contiene el paquete es correcto o no
	NivelPeso miPeso;		//Variable que indica qu� peso esta marcado en el paquete, o si este peso ni siquera est� marcado
	int peso;				//Variable que indica cuanto pesa el paquete para mostrar en la balanza
	bool fragil;			//Variable que indica si tiene un sello de fr�gil puesto

	//Bool que indica si el paquete es en realidad una carta, y si debe ser representado como tal en el renderizado
	bool carta;

	//Variables que debe modificar el jugador
	Calle calleMarcada;		//Variable que indica para qu� distrito ha sido etiquetado el paquete
	bool envuelto;			//Variable que indica si est� envuelto o no el paquete
};

