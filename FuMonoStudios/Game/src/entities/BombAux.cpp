#include "BombAux.h"
#include <architecture/Game.h>
#include <architecture/Entity.h>
#include <components/DelayedCallback.h>
#include <sistemas/SpecialObjectsFactory.h>
#include <architecture/GeneralData.h>

BombAux::BombAux() {
	bluePressed = false;
	kaboom = false;
	redPressed = false;
}

BombAux::~BombAux() {

}

void BombAux::RedPressed() {
	redPressed = true;

	if (!kaboom)
	{
		if (bluePressed) { // azul y luego rojo
			GoodEnding();
		}
		else { // primero rojo (mal)
			BadEnding();
		}
	}
}

void BombAux::BluePressed() {

	if (!kaboom)
	{
		if (bluePressed) { // dos azules
			BadEnding();
		}

		bluePressed = true;
	}
	
}

void BombAux::GoodEnding() {
	kaboom = true;

	gD().getNPCData(Secretario)->felicidad = FinalBien;

	SpecialObjectsFactory a = SpecialObjectsFactory();
	a.makeTransition();

	ecs::Entity* temp = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity();
	temp->addComponent<DelayedCallback>(3.2, []() {
		gm().requestChangeScene(ecs::sc::EXPLORE_SCENE, ecs::sc::END_SCENE);
		});
}

void BombAux::BadEnding() {
	kaboom = true;

	gD().getNPCData(Secretario)->felicidad = FinalMal;

	SpecialObjectsFactory a = SpecialObjectsFactory();
	a.makeExplosion();

	ecs::Entity* temp = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity();
	temp->addComponent<DelayedCallback>(4.2, []() {
		gm().requestChangeScene(ecs::sc::EXPLORE_SCENE, ecs::sc::END_SCENE);
		});
}