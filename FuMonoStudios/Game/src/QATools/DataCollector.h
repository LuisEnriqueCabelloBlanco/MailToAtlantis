#pragma once

#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <iostream>
#include <fstream>
#include "../../rapidCSV/rapidcsv.h"
#include <utils/Singleton.h>
#include <vector>
#include <components/Paquete.h>
#include <architecture/Game.h>
#include <unordered_map>
#ifdef QA_TOOLS

constexpr int NUMBER_OF_METRICS = 14;

/// <summary>
/// Clase abstracta de la que heredar para generar distintos csv en funcion de los datos a recopilar
/// </summary>
class DataCollector : public Singleton<DataCollector>
{
	friend Singleton<DataCollector>;

public:
	~DataCollector();

	void record();

	inline std::vector<float>& dataArray() { return dataArray_; }
	inline int& clicks() { return clicks_; }
	void recordPacage(Paquete*,bool);
	void recordNavigation(const std::string&);
	void recordNPC(int NPCid, int numDialog, int fel);
private:
	DataCollector();

	/// <summary>
	/// Documento sobre el que se escribe el csv
	/// </summary>
	rapidcsv::Document doc_;
	std::vector<float> dataArray_;
	int currentRow_;
	int clicks_;

	std::unordered_map<std::string, int> distIndx = {
		{"Hestia", 1},
		{"Artemisa", 2 },
		{"Demeter", 3 },
		{"Hefesto", 4 },
		{"Hermes", 5 },
		{"Apolo", 6 },
		{"Poseidon", 7 }

	};

};

inline DataCollector& dataCollector() {
	return *DataCollector::instance();
}
#endif // QA_TOOLS

