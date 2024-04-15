#pragma once
#include "../architecture/Scene.h"
#include "../sdlutils/VirtualTimer.h"

class EndWorkScene :public ecs::Scene {
public:
	EndWorkScene();
	virtual ~EndWorkScene();
	void init() override;
	void update() override;
private:
	void nextAnim(); // se encarga de llamar a las anims dependiendo de que anim toque (anim_)
	void animTextos(ecs::Entity* texto);	  // Efecto que le damos a fallos/aciertos
	void animNumeros(ecs::Entity* number); // Efecto incremento de numeros

	int anim_ = 1; // Lleva la cuenta de por que anim vamos
	double animCooldown_ = 1; // Tiempo en segundos entre cada anim
	VirtualTimer timer_;
};