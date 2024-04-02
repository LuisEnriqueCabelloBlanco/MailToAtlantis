#pragma once
#include "../utils/Singleton.h"
#include <unordered_map>

//enums globales
namespace pers{
	enum Personajes { SECRETARIO_REAL, VAGABUNDO, CONTABLE, ARTESANO, TAROTISA, SOLDADO, CAMPESINO };
	enum EstadosDeFelicidad { MINIMA, BAJA, INTERMEDIA, FELIZ, MAXIFELIZ };
}

using uint = unsigned int;
class Felicidad : public Singleton<Felicidad>
{
	friend Singleton<Felicidad>;
public:
	Felicidad();

	void aumentaFel(pers::Personajes p, uint f);
	void decrementaFel(pers::Personajes p, uint f);
	pers::EstadosDeFelicidad interpretaFel(pers::Personajes p) const;
	pers::Personajes getPersonaje(int p) {
		return pers::Personajes(p);
	}
private:
	std::unordered_map<pers::Personajes, int> felicidades_;
};

inline Felicidad& felicidad() {
	return *Felicidad::instance();
}