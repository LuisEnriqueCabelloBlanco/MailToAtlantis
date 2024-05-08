#include "BombAux.h"

BombAux::BombAux() {
	bluePressed = false;
}

BombAux::~BombAux() {

}

void BombAux::RedPressed() {
	if (bluePressed) {
		// final de 
	}
	else {
		// explotar y final de rip
	}
}

void BombAux::BluePressed() {
	bluePressed = true;
}