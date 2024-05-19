#pragma once
#include <iostream>

class save_Missing : public std::exception {
public:
	save_Missing(const char* msg):exception(msg) {}
};