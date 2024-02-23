#include "MainScene.h"
#include "../architecture/Entity.h"
#include <iostream>
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"
#include "../components/Render.h"
#include "../components/Clickeable.h"
#include "../components/DragAndDrop.h"
#include "../components/Trigger.h"
#include "../architecture/Game.h"
#include <string>
#include "../sdlutils/Texture.h"
#include "../components/PackageChecker.h"
#include "../components/Paquete.h"

ecs::MainScene::MainScene():Scene()
{
	
}

ecs::MainScene::~MainScene()
{
}


void sellar() {
	std::cout << "fumas ganja";
}

void ecs::MainScene::init()
{
	std::cout << "Hola Main" << std::endl;
	sdlutils().clearRenderer(build_sdlcolor(0xFFFFFFFF));
	//crear objetos
	/*
	// Caja CLicker
	Entity* Prueba2 = addEntity();
	Texture* sujetaplazas = &sdlutils().images().at("boxTest");
	float scale = 0.2;
	Transform* e = Prueba2->addComponent<Transform>(700.0f, 100.0f, sujetaplazas->width() * scale, sujetaplazas->height() * scale);
	RenderImage* nachos = Prueba2->addComponent<RenderImage>(sujetaplazas);
	auto clicker = Prueba2->addComponent<Clickeable>();
	Prueba2->addComponent<Trigger>();

	Prueba2->getComponent<Trigger>()->addCallback([]() {

		std::cout << "Activar Evento P2" << std::endl;

		});

	//TODO: probar que con un boton se puedan cargar otras escenas
	Callback cosa = []() {
		std::cout << "Click" << std::endl;
	};
	clicker->addEvent(cosa);

	// Dragable Box
	Entity* Prueba3 = addEntity(layer::BACKGROUND);
	Transform* tr = Prueba3->addComponent<Transform>(100.0f, 100.0f, sujetaplazas->width() * scale, sujetaplazas->height() * scale);
	RenderImage* rd = Prueba3->addComponent<RenderImage>(sujetaplazas);
	Prueba3->addComponent<DragAndDrop>();
	Prueba3->getComponent<Trigger>()->addCallback([]() {

		std::cout << "Activar Evento P3" << std::endl;

		});

	// Sello
	Entity* selloPrueba = addEntity(layer::DEFAULT);
	Texture* selloTexture = &sdlutils().images().at("selloTest");
	Transform* trSello = selloPrueba->addComponent<Transform>(700.0f, 100.0f, selloTexture->width() * scale, selloTexture->height() * scale);
	RenderImage* rd1 = selloPrueba->addComponent<RenderImage>(selloTexture);

	// Posici�n Relativa
	e->addChild(trSello);
	trSello->setRelativePos(100.0f, 100.0f);*/

	float scale = 0.2;

	//Paquete de prueba
	Entity* Paquet = addEntity(layer::BACKGROUND);
	Texture* texturaPaquet = &sdlutils().images().at("boxTest");
	Transform* trPq = Paquet->addComponent<Transform>(100.0f, 100.0f, texturaPaquet->width() * scale, texturaPaquet->height() * scale);
	RenderImage* rd = Paquet->addComponent<RenderImage>(texturaPaquet);
	Paquet->addComponent<Trigger>()->addCallback(sellar);
	Paquete* pqPq = Paquet->addComponent<Paquete>(Paquete::Demeter, Paquete::C1, Paquete::Alimento, 
		true, Paquete::Bajo, 20, false, false);
	DragAndDrop* drgPq = Paquet->addComponent<DragAndDrop>();

	// Sellador calle
	Entity* sellador = addEntity();
	Texture* selladorTextura = &sdlutils().images().at("selladorTest");
	Transform* trSellador = sellador->addComponent<Transform>(700, 700, selladorTextura->width() * scale, selladorTextura->height() * scale);
	Trigger* trgSell = sellador->addComponent<Trigger>();
	sellador->addComponent<RenderImage>(selladorTextura);
	sellador->addComponent<DragAndDrop>();


	//TODO: Pasar a una clase / metodo guapa guapa la generaci�n de las direciones
	//Direccion
	Entity* ent = addEntity();
	auto font = Font("recursos/fonts/Comic_Sans_MS.ttf", 16);
	std::string msg = "Tu vieja \n 001 \n Calle Lubina, La Sabia";
	SDL_Color color1 = build_sdlcolor(0xffffffff);
	SDL_Color color2 = build_sdlcolor(0x000000ff);
	Paquete* pacage = ent->addComponent<Paquete>(Paquete::Demeter, Paquete::C1, Paquete::Alimento,
		true, Paquete::Bajo, 20, false, false);
	Texture* text = new Texture(sdlutils().renderer(),pacage->getDirecction(), font, color2, color1, 200);
	Transform* trDir = ent->addComponent<Transform>(0, 0, 200, 70);
	ent->addComponent<RenderImage>(text);

	//Tuber�a
	Entity* tuberia = addEntity();
	Transform* trTub = tuberia->addComponent<Transform>(500, 100, 50, 50);
	Trigger* trgTub = tuberia->addComponent<Trigger>();
	PackageChecker* checker = tuberia->addComponent<PackageChecker>(Paquete::Apolo);
	trgTub->addCallback([&checker,&pqPq]() {
		if (checker->checkPackage(pqPq)) {
			std::cout << "WAA!  YA MADE IT!\n";
		}
		else {
			std::cout << "NUH UH\n";
		}
		});

	//Tuber�a2
	/*Entity* tuber2 = addEntity();
	Transform* trTb2 = tuber2->addComponent<Transform>(700, 100, 50, 50);
	Trigger* trgTb2 = tuber2->addComponent<Trigger>();
	PackageChecker* che = tuber2->addComponent<PackageChecker>(Paquete::Demeter);
	trgTb2->addCallback([&che, &pqPq]() {
		if (che->checkPackage(pqPq)) {
			std::cout << "WAA!  YA MADE IT!\n";
		}
		else {
			std::cout << "NUH UH\n";
		}
		});*/
}