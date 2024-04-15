#pragma once
#include "../architecture/Scene.h"

class EndWorkScene :public ecs::Scene {
public:
	EndWorkScene();
	virtual ~EndWorkScene();
	void init() override;
	void update() override;
private:
	void animTextos(ecs::Entity* corrects, ecs::Entity* fails);	  // Efecto que le damos a fallos/aciertos
	void animNumeros(ecs::Entity* nomina, ecs::Entity* totalMoney); // Efecto incremento de numeros

	double animCooldown_ = 1; // Tiempo en segundos entre cada anim
};