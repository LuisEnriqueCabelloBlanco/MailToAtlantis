#include "MainScene.h"
#include "../architecture/Entity.h"
#include <iostream>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>
#include "../sdlutils/SDLUtils.h"
#include "../components/Transform.h"
#include "../components/Render.h"
#include "../components/Clickeable.h"
#include "../components/DragAndDrop.h"
#include "../components/Trigger.h"
#include "../components/Wrap.h"
#include "../architecture/Game.h"
#include <string>
#include <list>
#include "../sdlutils/Texture.h"
#include "../components/PackageChecker.h"
#include "../sistemas/PaqueteBuilder.h"
#include "../components/Herramientas.h"
#include "../components/MultipleTextures.h"
#include "../components/Gravity.h"
#include "../components/MoverTransform.h"
#include "../architecture/Time.h"
#include "../architecture/GameConstants.h"
#include "../components/SelfDestruct.h"
#include "../architecture/GeneralData.h"
#include "../sistemas/ComonObjectsFactory.h"
#include "../components/Depth.h"
#include "../components/ErrorNote.h"
#include "../entities/ClockAux.h"

ecs::MainScene::MainScene():Scene(),fails_(0),correct_(0), timerPaused_(false)
{
	timer_ = MINIGAME_TIME;
#ifdef DEV_TOOLS
	stampsUnloked_= true;
	timeToAdd_ = 5;
#endif // DEV_TOOLS

}

ecs::MainScene::~MainScene()
{
}


void ecs::MainScene::update()
{
	Scene::update();
	if (!timerPaused_)
	{
		if (timer_ > 0) {
			timer_ -= Time::getDeltaTime();
		}
		else
			gm().requestChangeScene(ecs::sc::MAIN_SCENE, ecs::sc::END_WORK_SCENE);
	}
}

void ecs::MainScene::render()
{
	Scene::render();
#ifdef DEV_TOOLS
	ImGui::NewFrame();

	ImGui::Begin("Paquetes Scene Data");
	std::string time = "Current Game Time: " + std::to_string(timer_);
	ImGui::Text(time.c_str());
	std::string data = "Aciertos: " + std::to_string(correct_);
	ImGui::Text(data.c_str());
	data = "Fallos: " + std::to_string(fails_);
	ImGui::Text(data.c_str());
	data = "Pacage Level: " + std::to_string(generalData().getPaqueteLevel());
	ImGui::Text(data.c_str());
	ImGui::End();


	ImGui::Begin("Controls");
	if (ImGui::CollapsingHeader("Paquetes"))
	{
		ImGui::Checkbox("Next Pacage Correct",&nextPacageCorrect_);
		if (ImGui::Button("Create pacage")) {
			createPaquete(generalData().getPaqueteLevel());
		}
	}
	//Todavia no es funcinal ya que no hay forma actual de limitar las mecánicas
	if (ImGui::CollapsingHeader("Mecánicas"))
	{
		int lvl = generalData().getPaqueteLevel();
		ImGui::InputInt("Nivel del Paquete", &lvl);
		generalData().setPaqueteLevel(lvl);
		//ImGui::Checkbox("Sellos",&stampsUnloked_);
		//ImGui::Checkbox("Peso",&weightUnloked_);
		//ImGui::Checkbox("Cinta", &cintaUnloked_);
	}
	if (ImGui::CollapsingHeader("Tiempo")) {
		if (ImGui::Button("Reset Timer")) {
			timer_ = MINIGAME_TIME;
		}

		ImGui::InputInt("Aditional Seconds", &timeToAdd_);
		if (ImGui::Button("Add Time")) {
			timer_ += timeToAdd_;
		}
	}

	ImGui::End();
	ImGui::Render();

	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
#endif // DEV_TOOLS

}

void ecs::MainScene::init()
{
	std::cout << "Hola Main" << std::endl;
	sdlutils().clearRenderer(build_sdlcolor(0xFFFFFFFF));
	//crear objetos
	ComonObjectsFactory factory(this);
	timer_ = MINIGAME_TIME;
	// Fondo
	Entity* Fondo = addEntity(ecs::layer::BACKGROUND);
	Fondo->addComponent<Transform>(0, 0, sdlutils().width() / 1.25, sdlutils().height() / 1.25);
	Fondo->addComponent<RenderImage>(&sdlutils().images().at("fondoOficina"));

	createManual();

	createClock();

	createPaquete(generalData().getPaqueteLevel());

	// En el caso de que los tubos no estén ordenados, habrá que ordenarlos
	int numTubos = generalData().getTubesAmount(); // coge el numero de tubos que están desbloqueados
	int j = 0;
	for (int i = 0;i < numTubos; i++) {
		createTubo((Paquete::Distrito)i, true);
		j++;
	}
	//Creación de paquetes bloqueados
	for (int z = j; z < 7; z++) { //grande jose la los numeros magicos te la sabes
		createTubo((Paquete::Distrito)z, false);
	}

	//createSelladores();

	createInks();
  
  	//cinta envolver
	factory.setLayer(ecs::layer::TAPE);
	Entity* cinta = factory.createImage(Vector2D(560, 500), Vector2D(100, 150), &sdlutils().images().at("cinta"));
	cinta->addComponent<Gravity>();
	cinta->addComponent<DragAndDrop>();
	factory.setLayer(ecs::layer::DEFAULT);

	// papelera
	Entity* papelera = addEntity(ecs::layer::BIN);
	papelera->addComponent<Transform>(50, 650, 100, 150);
	papelera->addComponent<RenderImage>(&sdlutils().images().at("papelera"));
	Trigger* papTrig = papelera->addComponent<Trigger>();
	papTrig->addCallback([this](ecs::Entity* entRec) {
		Paquete* paqComp = entRec->getComponent<Paquete>();
		if (paqComp != nullptr)
		{
			if (paqComp->correcto())
			{
				createErrorMessage(paqComp, true, false);
			}
				
			else
			{ 
				generalData().correctPackage();
				correct_++;
			}
			
			entRec->setAlive(false);
			createPaquete(generalData().getPaqueteLevel());
		}
		});

	// A medida que se vaya avanzando en el desarrollo, se tendra que expandir esto de apajo para que en X dia suceda algo o aparezcan nuevas herramientas
	// Me gustaría que todo lo relacionado con los eventos de los dias y los paquetes y herramientas correspondientes estuviera documentado
	// En el miro había un esquema, pero este estaba con poco detalle, lo suyo es en gdd ver estas cosas, pero se va trabajando en ello
	int dia = generalData().getDia();
	if (dia > 0 && dia < 2) {
		Texture* texturaSellador = &sdlutils().images().at("selladorA");
		Entity* sellador = addEntity(ecs::layer::STAMP);
		Transform* transformSellador = sellador->addComponent<Transform>(560, 0, texturaSellador->width(), texturaSellador->height());
		transformSellador->setScale(0.4);
		RenderImage* renderSellador = sellador->addComponent<RenderImage>(texturaSellador);
		sellador->addComponent<Gravity>();
		sellador->addComponent<DragAndDrop>();
		sellador->addComponent<Herramientas>();
		sellador->getComponent<Herramientas>()->setFunctionality(TipoHerramienta::SelloCalleA);
	}
	else if (dia >= 2 && dia < 4) {
		Texture* texturaCinta = &sdlutils().images().at("cinta");
		Entity* cinta = addEntity();
		Transform* transformCinta = cinta->addComponent<Transform>(560, 0, texturaCinta->width() / 2, texturaCinta->height() / 2);
		RenderImage* renderCinta = cinta->addComponent<RenderImage>(texturaCinta);
		cinta->addComponent<Gravity>();
		cinta->addComponent<DragAndDrop>();
	}
	else if (dia >= 4 && dia < 6) {}
	else if (dia >= 6 && dia < 8) {}
	else if (dia >= 8 && dia < 10) {}

	/*switch (dia) {
	case 1: {
		Texture* texturaSellador1 = &sdlutils().images().at("sellador");
		Entity* sellador1 = addEntity();
		Transform* transformSellador1 = sellador1->addComponent<Transform>(460, 0, texturaSellador1->width() / 2, texturaSellador1->height() / 2);
		RenderImage* renderSellador1 = sellador1->addComponent<RenderImage>(texturaSellador1);
		sellador1->addComponent<Gravity>();
		sellador1->addComponent<DragAndDrop>();
		sellador1->addComponent<Herramientas>();
		sellador1->getComponent<Herramientas>()->setFunctionality(TipoHerramienta::SelloCalleA);
		break;
		}
	case 2: {
		Texture* texturaCinta1 = &sdlutils().images().at("cinta");
		Entity* cinta1 = addEntity();
		Transform* transformCinta1 = cinta1->addComponent<Transform>(460, 0, texturaCinta1->width() / 2, texturaCinta1->height() / 2);
		RenderImage* renderCinta1 = cinta1->addComponent<RenderImage>(texturaCinta1);
		cinta1->addComponent<Gravity>();
		cinta1->addComponent<DragAndDrop>();
		break;
		}
	}*/
}

void ecs::MainScene::close() {
	ecs::Scene::close();
	generalData().updateMoney(correct_,fails_);
}

void ecs::MainScene::createClock() {
	Entity* clock = addEntity(ecs::layer::BACKGROUND);
	clock->addComponent<ClockAux>(MINIGAME_TIME);
}

void ecs::MainScene::createInks() {

	// Tinta rojo (1)
	Entity* inkA = addEntity(layer::INK);
	Texture* inkATex = &sdlutils().images().at("tintaA");
	Transform* selloATR = inkA->addComponent<Transform>(300, 500, inkATex->width(), inkATex->height());

	selloATR->setScale(0.5);

	inkA->addComponent<RenderImage>(inkATex);

	Trigger* inkATri = inkA->addComponent<Trigger>();

	inkATri->addCallback([this](ecs::Entity* entRec) {

		if (entRec->getLayer() == ecs::layer::STAMP) {

			Herramientas* stampHerramienta = entRec->getComponent<Herramientas>();

			RenderImage* stampRender = entRec->getComponent<RenderImage>();

			stampHerramienta->setFunctionality(SelloCalleA);

			stampRender->setTexture(&sdlutils().images().at("selladorA"));

		}

	});

	


	// Tinta azul (2)
	Entity* inkB = addEntity(layer::INK);
	Texture* inkBTex = &sdlutils().images().at("tintaB");
	Transform* selloBTR = inkB->addComponent<Transform>(425, 500, inkBTex->width(), inkBTex->height());

	selloBTR->setScale(0.5);

	inkB->addComponent<RenderImage>(inkBTex);

	Trigger* inkBTri = inkB->addComponent<Trigger>();

	inkBTri->addCallback([this](ecs::Entity* entRec) {

		if (entRec->getLayer() == ecs::layer::STAMP) {

			Herramientas* stampHerramienta = entRec->getComponent<Herramientas>();

			RenderImage* stampRender = entRec->getComponent<RenderImage>();

			stampHerramienta->setFunctionality(SelloCalleB);

			stampRender->setTexture(&sdlutils().images().at("selladorB"));

		}

	});

	// Tinta verde (3)
	Entity* inkC = addEntity(layer::INK);
	Texture* inkCTex = &sdlutils().images().at("tintaC");
	Transform* selloCTR = inkC->addComponent<Transform>(550, 500, inkCTex->width(), inkCTex->height());

	selloCTR->setScale(0.5);

	inkC->addComponent<RenderImage>(inkCTex);

	Trigger* inkCTri = inkC->addComponent<Trigger>();

	inkCTri->addCallback([this](ecs::Entity* entRec) {

		if (entRec->getLayer() == ecs::layer::STAMP) {

			Herramientas* stampHerramienta = entRec->getComponent<Herramientas>();

			RenderImage* stampRender = entRec->getComponent<RenderImage>();

			stampHerramienta->setFunctionality(SelloCalleC);

			stampRender->setTexture(&sdlutils().images().at("selladorC"));

		}

		});

}


void ecs::MainScene::createErrorMessage(Paquete* paqComp, bool basura, bool tuboIncorrecto) {
	Entity* NotaErronea = addEntity(ecs::layer::BACKGROUND);	
	NotaErronea->addComponent<ErrorNote>(paqComp, basura, tuboIncorrecto);
	Texture* NotaTex = &sdlutils().images().at("notaError");
	Transform* selloATR = NotaErronea->addComponent<Transform>(100, 1400, NotaTex->width()*2, NotaTex->height()*2);
	selloATR->setScale(0.2f);
	NotaErronea->addComponent<DragAndDrop>(true, [NotaErronea]() {
		NotaErronea->addComponent<MoverTransform>(Vector2D(100, 1400), 0.5, Easing::EaseOutCubic, [NotaErronea]() {
			NotaErronea->setAlive(false);
			});
		});
	NotaErronea->addComponent<RenderImage>(NotaTex);
	
	//El texto de la nota
	Entity* texto_ = addEntity(ecs::layer::STAMP);
	Font* textFont = new Font("recursos/fonts/ARIAL.ttf", 40);
	Texture* textureText_ = new Texture(sdlutils().renderer(), NotaErronea->getComponent<ErrorNote>()->text_, *textFont, build_sdlcolor(0x000000ff), 500);
	Transform* distritoTr = texto_->addComponent<Transform>(25, 70, 250, 100);
	RenderImage* distritoRender = texto_->addComponent<RenderImage>();
	distritoRender->setTexture(textureText_);
	distritoTr->setParent(NotaErronea->getComponent<Transform>());

	NotaErronea->addComponent<MoverTransform>(Vector2D(100, 880), 0.5, Easing::EaseOutCubic);

}

void ecs::MainScene::createSelladores() {
	float scaleSelladores = 0.5f;

	// Sellador rojo (1)
	Entity* selloA = addEntity(layer::STAMP);
	Texture* selloATex = &sdlutils().images().at("selladorA");
	Transform* selloATR = selloA->addComponent<Transform>(100, 300, selloATex->width(), selloATex->height());
	selloATR->setScale(scaleSelladores);
	selloA->addComponent<DragAndDrop>(true, [selloA]() {
		selloA->addComponent<MoverTransform>(Vector2D(100,300), 0.5, Easing::EaseOutCubic);
		});
	selloA->addComponent<RenderImage>(selloATex);
	Herramientas* herrSelladorA = selloA->addComponent<Herramientas>();
	herrSelladorA->setFunctionality(SelloCalleA);
	
	// Sellador azul (2)
	Entity* selloB = addEntity(layer::STAMP);
	Texture* selloBTex = &sdlutils().images().at("selladorB");
	Transform* selloBTR = selloB->addComponent<Transform>(100, 410, selloBTex->width(), selloBTex->height());
	selloBTR->setScale(scaleSelladores);
	selloB->addComponent<DragAndDrop>(true, [selloB]() {
		selloB->addComponent<MoverTransform>(Vector2D(100, 410), 0.5, Easing::EaseOutCubic);
		});
	selloB->addComponent<RenderImage>(selloBTex);
	Herramientas* herrSelladorB = selloB->addComponent<Herramientas>();
	herrSelladorB->setFunctionality(SelloCalleB);

	// Sellador verde (3)
	Entity* selloC = addEntity(layer::STAMP);
	Texture* selloCTex = &sdlutils().images().at("selladorC");
	Transform* selloCTR = selloC->addComponent<Transform>(100, 520, selloCTex->width()
, selloCTex->height());
	selloCTR->setScale(scaleSelladores);
	selloC->addComponent<DragAndDrop>(true, [selloC]() {
		selloC->addComponent<MoverTransform>(Vector2D(100, 520), 0.5, Easing::EaseOutCubic);
		});
	selloC->addComponent<RenderImage>(selloCTex);
	Herramientas* herrSelladorC = selloC->addComponent<Herramientas>();
	herrSelladorC->setFunctionality(SelloCalleC);
}

void ecs::MainScene::createTubo(Paquete::Distrito dist, bool desbloqueado) {
	float scaleTubos = 0.3f;
	Entity* tuboEnt = addEntity(ecs::layer::BACKGROUND);
	Texture* texTubo = &sdlutils().images().at("tubo" + std::to_string(dist + 1));

	Transform* tuboTr = tuboEnt->addComponent<Transform>(75 + (220 * dist), -40, texTubo->width(), texTubo->height());
	tuboTr->setScale(scaleTubos);
	tuboEnt->addComponent<RenderImage>(texTubo);

	if (desbloqueado) {
		Trigger* tuboTri = tuboEnt->addComponent<Trigger>();
		PackageChecker* tuboCheck = tuboEnt->addComponent<PackageChecker>();
		tuboTri->addCallback([this, dist, tuboCheck](ecs::Entity* entRec) {
			//comprobamos si es un paquete
			Transform* entTr = entRec->getComponent<Transform>();
		if (entRec->getComponent<Paquete>() != nullptr) {
			entRec->removeComponent<Gravity>();
			entRec->addComponent<MoverTransform>( // animación básica del paquete llendose
				entTr->getPos() + Vector2D(0, -600), 1.5, Easing::EaseOutCubic);
			entRec->addComponent<SelfDestruct>(1, [this]() {
				generalData().correctPackage();
			createPaquete(generalData().getPaqueteLevel());
				});
			if (tuboCheck->checkPackage(entRec->getComponent<Paquete>())) {
				correct_++;
			}
			else {
				fails_++;				
				if (dist == entRec->getComponent<Paquete>()->getDistrito()) {
					createErrorMessage(entRec->getComponent<Paquete>(), false, false);					
				}
				else
				{
					createErrorMessage(entRec->getComponent<Paquete>(), false, true);
				}
			}

		}

			});
		tuboCheck->addCondition([dist](Paquete* paqRec) -> bool {
			return paqRec->getDistrito() == dist;
			});
	}
	else {
		Entity* cruzEnt = addEntity(ecs::layer::UI);
		Texture* texCruz = &sdlutils().images().at("cruz");

		Transform* cruzTr = cruzEnt->addComponent<Transform>(75 + (220 * dist), 120, texCruz->width(), texCruz->height());
		cruzTr->setScale(0.2f);
		cruzEnt->addComponent<RenderImage>(texCruz);
	}

#ifdef _DEBUG
	std::cout << "Creado el tubo  " << dist << std::endl;
#endif // _DEBUG

}

void ecs::MainScene::createManual()
{
	ComonObjectsFactory fact(this);
	Entity* manual = addEntity(ecs::layer::MANUAL);
	//se puede hacer un for
	Texture* manualTexture = &sdlutils().images().at("book1");
	Texture* manualTexture2 = &sdlutils().images().at("book2");
	Texture* manualTexture3 = &sdlutils().images().at("book3");
	Texture* manualTexture4 = &sdlutils().images().at("book4");
	Texture* manualTexture5 = &sdlutils().images().at("book5");
	Texture* buttonTexture = &sdlutils().images().at("flechaTest");

	std::vector<Texture*> bookTextures = {
		manualTexture,
		manualTexture2,
		manualTexture3,
		manualTexture4,
		manualTexture5
	};
	float scaleManual = 0.075;
	Transform* manualTransform = manual->addComponent<Transform>(500.0f, 500.0f, manualTexture->width(), manualTexture->height());
	manualTransform->setScale(scaleManual);
	RenderImage* manualRender = manual->addComponent<RenderImage>();
	manual->addComponent<Gravity>();
	manual->addComponent<DragAndDrop>(true);
	MultipleTextures* multTextures = manual->addComponent<MultipleTextures>(bookTextures);
	manualRender->setTexture(multTextures->getCurrentTexture());


	Vector2D buttonSize(100, 40);
	fact.setLayer(ecs::layer::FOREGROUND);

	auto next = [multTextures]() {multTextures->nextTexture();};
	auto right = fact.createImageButton(Vector2D(400, 300), buttonSize, buttonTexture, next);
	right->getComponent<Transform>()->setParent(manualTransform);

	auto previous = [multTextures]() {multTextures->previousTexture();};
	auto left = fact.createImageButton(Vector2D(100, 300), buttonSize, buttonTexture, previous);
	left->getComponent<Transform>()->setParent(manualTransform);


	fact.setLayer(ecs::layer::DEFAULT);

	manual->addComponent<Depth>();
}

void ecs::MainScene::createPaquete (int lv) {
	float paqueteScale = 0.25f;
	Entity* paqEnt = addEntity (ecs::layer::PACKAGE);

	Texture* texturaPaquet = &sdlutils ().images ().at ("boxTest");

	std::vector<Texture*> textures = {
		texturaPaquet,
		&sdlutils().images().at("caja25"),
		&sdlutils().images().at("caja50"),
		&sdlutils().images().at("caja75"),
		&sdlutils().images().at("caja100")
	};

	Transform* trPq = paqEnt->addComponent<Transform> (1600.0f, 600.0f, texturaPaquet->width (), texturaPaquet->height ());
	trPq->setScale(paqueteScale);+
	paqEnt->addComponent<Depth>();
	RenderImage* rd = paqEnt->addComponent<RenderImage> (texturaPaquet);
	paqEnt->addComponent<Gravity>();
	DragAndDrop* drgPq = paqEnt->addComponent<DragAndDrop>(true);
	std::list<int> route {pointRoute::LeftUp, pointRoute::MiddleUp, pointRoute::MiddleMid, pointRoute::MiddleDown, pointRoute::RightDown};

	MultipleTextures* multTexturesPaq = paqEnt->addComponent<MultipleTextures>(textures);

	multTexturesPaq->initComponent();

	//Wrap debe ir despues del Transform, Trigger y Multitextures
	paqEnt->addComponent<Wrap>(20, 0, route);


	PaqueteBuilder a;
	a.paqueteRND (lv, paqEnt);

	// añadimos que pueda ser interactuado por selladores
	paqEnt->getComponent<Trigger>()->addCallback([paqEnt](ecs::Entity* entRec) {
		Herramientas* herrEnt = entRec->getComponent<Herramientas>();
		if (herrEnt != nullptr)
		{
			herrEnt->interact(paqEnt);
		}
		});

	paqEnt->addComponent<MoverTransform>(Vector2D(1200,600), 1, EaseOutBack);
}