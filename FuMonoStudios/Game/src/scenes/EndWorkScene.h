#pragma once
#include "../architecture/Scene.h"

class EndWorkScene :public ecs::Scene {
public:
	EndWorkScene();
	virtual ~EndWorkScene();
	void init() override;
private:
	void animTexto(Entity* text);	  // Efecto que le damos a fallos/aciertos
	void animNumeros(Entity* number); // Efecto incremento de numeros

	// Ajustes
	double animCooldown_ = 1; // Tiempo en segundos entre cada anim
	double animVelocity_ = 2; // Tiempo en segundos que tardan las anims
};