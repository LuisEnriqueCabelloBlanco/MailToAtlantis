#include "SpecialObjectsFactory.h"
#include <components/Render.h>
#include <scenes/MainScene.h>
#include <sdlutils/SDLUtils.h>
#include <components/DragAndDrop.h>

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
}

void SpecialObjectsFactory::makePapelAgujeros() {
	ecs::Entity* ent = scene->addEntity(ecs::layer::OFFICEELEMENTS);
	Transform* tr = ent->addComponent<Transform>(1400, 600, 320, 245);
	ent->addComponent<RenderImage>(&sdlutils().images().at("papelAgujeros"));
	ent->addComponent<DragAndDrop>(false, "arrastrar");
	ent->addComponent<Gravity>();
	ent->addComponent<Depth>();
}