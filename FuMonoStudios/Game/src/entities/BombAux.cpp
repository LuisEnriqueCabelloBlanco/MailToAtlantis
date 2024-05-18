#include "BombAux.h"

BombAux::BombAux() {
	bluePressed = false;
}

BombAux::~BombAux() {

}

void BombAux::RedPressed() {
	if (bluePressed) {
		// final bien
		gm().requestChangeScene(ecs::sc::EXPLORE_SCENE, ecs::sc::MAIN_SCENE);
	}
	else {
		gm().requestChangeScene(ecs::sc::EXPLORE_SCENE, ecs::sc::DEATH_SCENE);
		// explotar y final de rip
	}
}

void BombAux::BluePressed() {
	bluePressed = true;
}