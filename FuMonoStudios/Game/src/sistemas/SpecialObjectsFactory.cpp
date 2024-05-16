#include "SpecialObjectsFactory.h"
#include <components/Render.h>
#include <scenes/MainScene.h>
#include <sdlutils/SDLUtils.h>
#include <components/DragAndDrop.h>
#include <components/Clickeable.h>
#include <entities/BombAux.h>

SpecialObjectsFactory::SpecialObjectsFactory(ecs::Scene* sc) {
	scene = sc;
}

SpecialObjectsFactory::~SpecialObjectsFactory() {
	for (auto t : createdTextures)
		delete t;
}



void SpecialObjectsFactory::setupDayObjects() {

	// lista vagabundo
	if (gD().getNPCData(Vagabundo)->numMisionesAceptadas == 4 && gD().getNPCData(Vagabundo)->postConversation)
		makeListaVagabundo();
	if ((gD().getNPCData(Vagabundo)->numMisionesAceptadas == 4 ||
		gD().getNPCData(Vagabundo)->numMisionesAceptadas == 5) && gD().getNPCData(Vagabundo)->postConversation)
		makePapelAgujeros();
	if (generalData().getNPCData(Secretario)->numMisionesAceptadas == 3)
		makeListaSecretario(true);
	if (generalData().getNPCData(Secretario)->numMisionesAceptadas == 4)
		makeListaSecretario(false);

	makeBomba();
}

void SpecialObjectsFactory::makeListaVagabundo() {
	ecs::Entity* ent = scene->addEntity(ecs::layer::OFFICEELEMENTS);

	Transform* tr = ent->addComponent<Transform>(1200,600, 250, 400);
	ent->addComponent<RenderImage>(&sdlutils().images().at("notaError"));
	ent->addComponent<DragAndDrop>(false, "arrastrar");
	ent->addComponent<Gravity>();
	ent->addComponent<Depth>();

	ecs::Entity* texto = scene->addEntity(ecs::layer::OFFICEELEMENTS);
	Transform* textoTr = texto->addComponent<Transform>(15, 70, 1, 1);
	textoTr->setParent(tr);
	const std::string textoString = "        EL REY ES CORRUPTO.\n       SOMOS RESISTENCIA.\nVENDRAN GRANDES CAMBIOS SI\n           NOS UNIMOS.\nAYUDA Y FORMARAS PARTE DEL\n               CAMBIO\nELIMINA LOS PAQUETES DE LOS\n        NOMBRES DE LA LISTA.\n       LA LECHUZA AGUARDA.\n\n-FRANCIS GOMORRA\n-DEMETRIUS DC\n-ANNA GOMORRA\n-SILVIO PERALTA";
	Texture* textTexture = new Texture(
		sdlutils().renderer(), textoString, sdlutils().fonts().at("simpleHandmade20"),
		build_sdlcolor(0x00000000ff), 250);
	createdTextures.push_back(textTexture);
	RenderImage* textoRnd = texto->addComponent<RenderImage>(textTexture);
	textoTr->setWidth(textoRnd->getCurrentTexture()->width());
	textoTr->setHeith(textoRnd->getCurrentTexture()->height());

	ecs::Entity* entSello = scene->addEntity(ecs::layer::OFFICEELEMENTS);

	Transform* trSello = entSello->addComponent<Transform>(15, 30, 45, 45);
	entSello->addComponent<RenderImage>(&sdlutils().images().at("selloLechuzas"));
	trSello->setParent(tr);
}

void SpecialObjectsFactory::makeListaSecretario(bool first) {
	if (first) 
	{
		ecs::Entity* ent = scene->addEntity(ecs::layer::OFFICEELEMENTS);

		Transform* tr = ent->addComponent<Transform>(1100, 600, 250, 200);
		ent->addComponent<RenderImage>(&sdlutils().images().at("notaError"));
		ent->addComponent<DragAndDrop>(false, "arrastrar");
		ent->addComponent<Gravity>();
		ent->addComponent<Depth>();

		ecs::Entity* texto = scene->addEntity(ecs::layer::OFFICEELEMENTS);
		Transform* textoTr = texto->addComponent<Transform>(15, 20, 1, 1);
		textoTr->setParent(tr);
		const std::string textoString = "   POSIBLES MIEMBROS DE LAS\n              LECHUZAS.\n     ELIMINAR SUS PAQUETES\n\n-Dr Merluzo\n-Anxoa Rios\nGorg Lotus";
		Texture* textTexture = new Texture(
			sdlutils().renderer(), textoString, sdlutils().fonts().at("simpleHandmade20"),
			build_sdlcolor(0x00000000ff), 250);
		createdTextures.push_back(textTexture);
		RenderImage* textoRnd = texto->addComponent<RenderImage>(textTexture);
		textoTr->setWidth(textoRnd->getCurrentTexture()->width());
		textoTr->setHeith(textoRnd->getCurrentTexture()->height());
	}
	else
	{
		ecs::Entity* ent = scene->addEntity(ecs::layer::OFFICEELEMENTS);

		Transform* tr = ent->addComponent<Transform>(1300, 600, 250, 200);
		ent->addComponent<RenderImage>(&sdlutils().images().at("notaError"));
		ent->addComponent<DragAndDrop>(false, "arrastrar");
		ent->addComponent<Gravity>();
		ent->addComponent<Depth>();

		ecs::Entity* texto = scene->addEntity(ecs::layer::OFFICEELEMENTS);
		Transform* textoTr = texto->addComponent<Transform>(15, 20, 1, 1);
		textoTr->setParent(tr);
		const std::string textoString = "   POSIBLES MIEMBROS DE LAS\n              LECHUZAS.\n     ELIMINAR SUS PAQUETES\n\n-Anemos Togakawa\n-Merla Flota\nSalvador Coral";
		Texture* textTexture = new Texture(
			sdlutils().renderer(), textoString, sdlutils().fonts().at("simpleHandmade20"),
			build_sdlcolor(0x00000000ff), 250);
		createdTextures.push_back(textTexture);
		RenderImage* textoRnd = texto->addComponent<RenderImage>(textTexture);
		textoTr->setWidth(textoRnd->getCurrentTexture()->width());
		textoTr->setHeith(textoRnd->getCurrentTexture()->height());
	}
}

void SpecialObjectsFactory::makePapelAgujeros() {
	ecs::Entity* ent = scene->addEntity(ecs::layer::OFFICEELEMENTS);
	Transform* tr = ent->addComponent<Transform>(1400, 600, 320, 245);
	ent->addComponent<RenderImage>(&sdlutils().images().at("papelAgujeros"));
	ent->addComponent<DragAndDrop>(false, "arrastrar");
	ent->addComponent<Gravity>();
	ent->addComponent<Depth>();
}

ecs::Entity* SpecialObjectsFactory::makeBomba() {
	ecs::Entity* entPaq = scene->addEntity(ecs::layer::PACKAGE);

	Transform* paqueteTr = entPaq->addComponent<Transform>(1600, 600, 450, 245);
	entPaq->addComponent<RenderImage>(&sdlutils().images().at("bomba"));
	entPaq->addComponent<DragAndDrop>(false, "arrastrar");
	entPaq->addComponent<Gravity>();
	entPaq->addComponent<Depth>();
	BombAux* bombAux = entPaq->addComponent<BombAux>();

	ecs::Entity* botonRojo = scene->addEntity(ecs::layer::PACKAGE);
	Transform* botonRojoTr = botonRojo->addComponent<Transform>(170, 82, 30, 40);
	botonRojoTr->setParent(paqueteTr);
	Clickeable* clRojo = botonRojo->addComponent<Clickeable>("click");
	clRojo->addEvent([bombAux] {
		bombAux->RedPressed();
		});

	ecs::Entity* botonAzul = scene->addEntity(ecs::layer::PACKAGE);
	Transform* botonAzulTr = botonAzul->addComponent<Transform>(170, 140, 30, 40);
	botonAzulTr->setParent(paqueteTr);
	Clickeable* clAzul = botonAzul->addComponent<Clickeable>("click");
	clAzul->addEvent([bombAux] {
		bombAux->BluePressed();
		});
	return entPaq;
}