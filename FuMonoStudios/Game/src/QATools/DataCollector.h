#pragma once
#include <iostream>
#include <fstream>
#include "../../rapidCSV/rapidcsv.h"
#include <utils/Singleton.h>
#include <vector>

constexpr int NUMBER_OF_METRICS = 10;

/// <summary>
/// Clase abstracta de la que heredar para generar distintos csv en funcion de los datos a recopilar
/// </summary>
class DataCollector : public Singleton<DataCollector>
{
	friend Singleton<DataCollector>;

public:
	~DataCollector() {
		doc_.Save();
	};

	void record();

	inline std::vector<float>& dataArray() { return dataArray_; }
	inline int& clicks() { return clicks_; }

private:
	DataCollector();

	/// <summary>
	/// Documento sobre el que se escribe el csv
	/// </summary>
	rapidcsv::Document doc_;
	std::vector<float> dataArray_;
	int currentRow_;
	int clicks_;

};

inline DataCollector& dataCollector() {
	return *DataCollector::instance();
}

