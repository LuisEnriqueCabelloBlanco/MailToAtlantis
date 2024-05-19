#include "SpecialObjectsFactory.h"
#include <scenes/MainScene.h>
#include <components/Clickeable.h>
#include <entities/BombAux.h>
#include <components/DelayedCallback.h>

SpecialObjectsFactory::SpecialObjectsFactory() {

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
	if (gD().getNPCData(Secretario)->numMisionesAceptadas == 3 && gD().getNPCData(Secretario)->postConversation)
		makeListaSecretario(true);
	if (gD().getNPCData(Secretario)->numMisionesAceptadas == 4 && gD().getNPCData(Secretario)->postConversation)
		makeListaSecretario(false);

	if (gD().getNPCData(Vagabundo)->numMisionesAceptadas == 10 && gD().getNPCData(Vagabundo)->postConversation)
		makePolvos();
}

void SpecialObjectsFactory::makeListaVagabundo() {
	ecs::Entity* ent = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::OFFICEELEMENTS);

	Transform* tr = ent->addComponent<Transform>(1200,600, 250, 400);
	ent->addComponent<RenderImage>(&sdlutils().images().at("notaError"));
	ent->addComponent<DragAndDrop>(false, "arrastrar");
	ent->addComponent<Gravity>();
	ent->addComponent<Depth>();

	ecs::Entity* texto = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::OFFICEELEMENTS);
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

	ecs::Entity* entSello = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::OFFICEELEMENTS);

	Transform* trSello = entSello->addComponent<Transform>(15, 30, 45, 45);
	entSello->addComponent<RenderImage>(&sdlutils().images().at("selloLechuzas"));
	trSello->setParent(tr);
}

void SpecialObjectsFactory::makeListaSecretario(bool first) {
	if (first) 
	{
		ecs::Entity* ent = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::OFFICEELEMENTS);

		Transform* tr = ent->addComponent<Transform>(1100, 600, 250, 200);
		ent->addComponent<RenderImage>(&sdlutils().images().at("notaError"));
		ent->addComponent<DragAndDrop>(false, "arrastrar");
		ent->addComponent<Gravity>();
		ent->addComponent<Depth>();

		ecs::Entity* texto = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::OFFICEELEMENTS);
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
		ecs::Entity* ent = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::OFFICEELEMENTS);

		Transform* tr = ent->addComponent<Transform>(1300, 600, 250, 200);
		ent->addComponent<RenderImage>(&sdlutils().images().at("notaError"));
		ent->addComponent<DragAndDrop>(false, "arrastrar");
		ent->addComponent<Gravity>();
		ent->addComponent<Depth>();

		ecs::Entity* texto = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::OFFICEELEMENTS);
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
	ecs::Entity* ent = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::OFFICEELEMENTS);
	Transform* tr = ent->addComponent<Transform>(1400, 600, 320, 245);
	ent->addComponent<RenderImage>(&sdlutils().images().at("papelAgujeros"));
	ent->addComponent<DragAndDrop>(false, "arrastrar");
	ent->addComponent<Gravity>();
	ent->addComponent<Depth>();
}

ecs::Entity* SpecialObjectsFactory::makeBomba() {
	ecs::Entity* entPaq = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::PACKAGE);

	Transform* paqueteTr = entPaq->addComponent<Transform>(1600, 600, 450, 245);
	entPaq->addComponent<RenderImage>(&sdlutils().images().at("bomba"));
	entPaq->addComponent<DragAndDrop>(false, "arrastrar");
	entPaq->addComponent<Gravity>();
	entPaq->addComponent<Depth>();
	BombAux* bombAux = entPaq->addComponent<BombAux>();

	ecs::Entity* botonRojo = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::PACKAGE);
	Transform* botonRojoTr = botonRojo->addComponent<Transform>(170, 82, 30, 40);
	botonRojoTr->setParent(paqueteTr);
	Clickeable* clRojo = botonRojo->addComponent<Clickeable>("click");
	clRojo->addEvent([bombAux] {
		bombAux->RedPressed();
		});

	ecs::Entity* botonAzul = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::PACKAGE);
	Transform* botonAzulTr = botonAzul->addComponent<Transform>(170, 140, 30, 40);
	botonAzulTr->setParent(paqueteTr);
	Clickeable* clAzul = botonAzul->addComponent<Clickeable>("click");
	clAzul->addEvent([bombAux] {
		bombAux->BluePressed();
		});
	return entPaq;
}

ecs::Entity* SpecialObjectsFactory::makePolvos() {
	ecs::Entity* entPaq = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::PACKAGE);

	Transform* paqueteTr = entPaq->addComponent<Transform>(1300, 600, 300, 250);
	entPaq->addComponent<RenderImage>(&sdlutils().images().at("boxTest"));
	entPaq->addComponent<DragAndDrop>(false, "arrastrar");
	entPaq->addComponent<Gravity>();
	entPaq->addComponent<Depth>();

	ecs::Entity* selloLechuzas = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::PACKAGE);
	Transform* selloTr = selloLechuzas->addComponent<Transform>(50,80, 75, 75);
	selloTr->setParent(paqueteTr);
	selloLechuzas->addComponent<RenderImage>(&sdlutils().images().at("selloLechuzas"));

	ecs::Entity* botonAbrir = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::PACKAGE);
	Transform* botonTr = botonAbrir->addComponent<Transform>(0, 0, 300, 150);
	botonTr->setParent(paqueteTr);
	Clickeable* clBoton = botonAbrir->addComponent<Clickeable>("click");
	clBoton->addEvent([this, paqueteTr, entPaq](){
		ecs::Entity* polvo = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::STAMP);
		polvo->addComponent<Transform>(paqueteTr->getCenter().getX(),
			paqueteTr->getCenter().getY(), 200, 200);
		polvo->addComponent<RenderImage>(&sdlutils().images().at("polvos"));
		polvo->addComponent<DragAndDrop>(false, "arrastrar");
		polvo->addComponent<Gravity>();
		polvo->addComponent<Depth>();
		Herramientas* herr = polvo->addComponent<Herramientas>();
		herr->setFunctionality(Polvos);
		entPaq->setAlive(false);
		});

	return entPaq;
}

void SpecialObjectsFactory::makeDeathTransition() {
	ecs::Entity* eyeBottom = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::UI);
	eyeBottom->addComponent<Transform>(0, 1500,
		1920, 1080);
	eyeBottom->addComponent<RenderImage>(&sdlutils().images().at("ojo"));
	MoverTransform* moverTrBottom = eyeBottom->addComponent<MoverTransform>(Easing::EaseOutBack);
	moverTrBottom->setFinalPos(Vector2D(0, 1080 / 2));
	moverTrBottom->setMoveTime(2);
	moverTrBottom->enable();

	ecs::Entity* eyeTop = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::UI);
	Transform* eyeTopTr = eyeTop->addComponent<Transform>(0, -1500,
		1920, 1080);
	eyeTopTr->setFlip(SDL_FLIP_VERTICAL);
	eyeTop->addComponent<RenderImage>(&sdlutils().images().at("ojo"));
	MoverTransform* moverTrTop = eyeTop->addComponent<MoverTransform>(Easing::EaseOutBack);
	moverTrTop->setFinalPos(Vector2D(0, (1080 / 2) - 1080));
	moverTrTop->setMoveTime(2);
	moverTrTop->enable();

	eyeBottom->addComponent<DelayedCallback>(2.1, [this, moverTrBottom, moverTrTop]() {
		moverTrBottom->setFinalPos(Vector2D(0, (1080 / 2) - 400));
		moverTrBottom->setMoveTime(2);
		moverTrBottom->enable();
		moverTrTop->setFinalPos(Vector2D(0, (1080 / 2) - 1080 + 400));
		moverTrTop->setMoveTime(2);
		moverTrTop->enable();
		});
}

void SpecialObjectsFactory::makeTransition() {
	ecs::Entity* cortina = gm().getScene(ecs::sc::MAIN_SCENE)->addEntity(ecs::layer::UI);
	Transform* tr = cortina->addComponent<Transform>(0, -2160,
		1920, 2160);
	tr->setFlip(SDL_FLIP_VERTICAL);
	cortina->addComponent<RenderImage>(&sdlutils().images().at("ojo"));
	MoverTransform* moverTrBottom = cortina->addComponent<MoverTransform>(Easing::EaseOutBack);
	moverTrBottom->setFinalPos(Vector2D(0, 0));
	moverTrBottom->setMoveTime(3);
	moverTrBottom->enable();
}