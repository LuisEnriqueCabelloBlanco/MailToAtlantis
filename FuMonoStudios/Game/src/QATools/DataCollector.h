#pragma once
#include <iostream>
#include <fstream>
#include "../../rapidCSV/rapidcsv.h"
/// <summary>
/// Clase abstracta de la que heredar para generar distintos csv en funcion de los datos a recopilar
/// </summary>
class DataCollector
{
protected:
	DataCollector(const std::string& docPath);
	virtual ~DataCollector() {
		doc.Save();
		//dataFile.close();
	};
	/// <summary>
	/// Caracter de separacion para el scv
	/// </summary>
	const char SEPARATOR = ';';
	/// <summary>
	/// Fichero en el que se cargar� la informacion
	/// </summary>
	std::ofstream dataFile;

	rapidcsv::Document doc;
};

