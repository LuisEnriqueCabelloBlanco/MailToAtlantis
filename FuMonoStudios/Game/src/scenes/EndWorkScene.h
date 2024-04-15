#pragma once
#include "../architecture/Scene.h"

class EndWorkScene :public ecs::Scene {
public:
	EndWorkScene();
	virtual ~EndWorkScene();
	void init() override;
private:
	void animTextos(ecs::Entity* corrects, ecs::Entity* fails);	  // Efecto que le damos a fallos/aciertos
	void animNumeros(ecs::Entity* number); // Efecto incremento de numeros

	// Ajustes
	double animCooldown_ = 1; // Tiempo en segundos entre cada anim
};