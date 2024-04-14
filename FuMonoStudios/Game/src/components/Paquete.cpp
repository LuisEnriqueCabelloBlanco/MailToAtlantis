
#include "Paquete.h"
#include "../json/JSON.h"
#include <memory>
#include <iostream>
#include "../architecture/Entity.h"
#include "Render.h"
#include "../architecture/Scene.h"
#include "../sdlutils/InputHandler.h"
#include <bitset>


// cleon: 962 gatitos acaban de morir. con dolor.
const int nivelFragil = 3;
const int nivelPeso = 2;
const int nivelSellos = 1;

const int desviacionPeso = 15;
const int ligeroMax = 25;
const int medioMax = 50;
const int pesadoMax = 75;

Paquete::Paquete(Paquete& otherPckg)
{
	miDistrito_ = otherPckg.miDistrito_;
	miCalle_ = otherPckg.miCalle_;
	miRemitente_ = otherPckg.miRemitente_;
	miTipo_ = otherPckg.miTipo_;
	selloCorrecto_ = otherPckg.selloCorrecto_;
	miPeso_ = otherPckg.miPeso_;
	peso_ = otherPckg.peso_;
	fragil_ = otherPckg.fragil_;
	carta_ = otherPckg.carta_;
	envuelto_ = otherPckg.envuelto_;
	calleMarcada_ = otherPckg.calleMarcada_;
	nombreCalle_ = otherPckg.nombreCalle_;
}

Paquete::Paquete(Distrito dis, Calle c, const std::string& nombreCalle, const std::string& remitente, TipoPaquete Tp, bool corr, NivelPeso Np, int p, bool f, bool cart) :
	miDistrito_(dis), miCalle_(c), miRemitente_(remitente),miTipo_(Tp),selloCorrecto_(corr), 
	miPeso_(Np), peso_(p), fragil_(f), carta_(cart),envuelto_(false), calleMarcada_(Erronea), nombreCalle_(nombreCalle) {

}

Paquete::~Paquete() {

}

void Paquete::initComponent() {
}

bool Paquete::bienSellado() const{
	bool bienSellado_ = calleMarcada_ != Erronea && calleMarcada_ == miCalle_;
	return bienSellado_ && correctFragile();
}
bool Paquete::pesoCorrecto() const {
	bool result = true;
	if (miPeso_ != Ninguno) {	//Si tiene un sello de pesado y su peso no est� entre los valores indicados, el paquete no es correcto
		if (miPeso_ == Bajo) {
			if (peso_ > ligeroMax) result = false;
		}
		else if (miPeso_ == Medio) {
			if (peso_ < ligeroMax || peso_ > medioMax) result = false;
		}
		else if (miPeso_ == Alto) {
			if (peso_ < medioMax) result = false;
		}
	}
	return result;
}
bool Paquete::correctFragile() const
{
	return (!fragil_ && !envuelto_) || (fragil_ && envuelto_);
}
bool Paquete::correcto() const{ 
	//M�todo que comprueba si el paquete habia sido generado sin errores (AKA: Si da false, eso significa que se tendr�a que devolver al remitente)
	bool correcto = miCalle_ != Erronea&&
		miDistrito_ != Erroneo&&
		selloCorrecto_&&
		pesoCorrecto()
		;
	return correcto;	//Si ha superdado todas las pruebas exitosamente, el paquete ser� correcto y devolver� true. Si en alg�n momento ha fallado, devolver� false
}

void Paquete::sellarCalle(Calle sello, Transform* trSellador) {

	Vector2D posSellador = trSellador->getPos();
	// solo puedes sellar una vez 
	if (sello != Erronea && calleMarcada_ == Erronea)
	{
		calleMarcada_ = sello;
		Transform* paqTr = ent_->getComponent<Transform>();

		//Creamos la entidad sello
		ecs::Entity* selloEnt = ent_->getMngr()->addEntity(ecs::layer::PACKAGE);
		//Textura en funcion de tipo calle
		Texture* selloEntTex = &sdlutils().images().at(
			(std::string)"sello" += 
			(std::string)(sello == C1 ? "0" : sello == C2 ? "1" : "2"));

		//creamos transform y colocamos el sello en el centro del sellador
		float scale = 0.2f;
		Transform* selloEntTr = selloEnt->addComponent<Transform>
			(posSellador.getX() + selloEntTex->width() / 2 * scale - paqTr->getPos().getX(),
			posSellador.getY() + selloEntTex->height() / 2 * scale - paqTr->getPos().getY(),
			selloEntTex->width() * scale, selloEntTex->height() * scale);
		selloEnt->addComponent<RenderImage>(selloEntTex);
		selloEntTr->setParent(paqTr);
	}
}

void Paquete::puntosRojos(int routeID) {
	Transform* paqTr = ent_->getComponent<Transform>();

	// Creamos la entidad para el punto 
	Texture* puntoRojoTex = &sdlutils().images().at("puntoRojo");
	Texture* puntoVerdeTex = &sdlutils().images().at("puntoVerde");
	float scale = 0.2f;

	if (puntoRojoTex != nullptr) {
		// Obtener las dimensiones del paquete
		float paqWidth = paqTr->getRect().w;
		float paqHeight = paqTr->getRect().h;

		// Calcular las nuevas coordenadas relativas de cada punto rojo (/60 un poco raro, habria q revisarlo)
		float offsetX = paqWidth / 60;
		float offsetY = paqHeight / 60;

		// Definir las posiciones de los puntos rojos con el desplazamiento
		std::vector<Vector2D> redPoints = {

			  Vector2D(-offsetX, -offsetY), // Superior izquierda
			  Vector2D(paqWidth + offsetX, -offsetY), // Superior derecha
			  Vector2D(-offsetX, paqHeight + offsetY), // Inferior izquierda
			  Vector2D(paqWidth + offsetX, paqHeight + offsetY), // Inferior derecha
			  Vector2D(paqWidth / 2, -offsetY), // Medio superior
			  Vector2D(paqWidth / 2, paqHeight + offsetY), // Medio inferior
			  Vector2D(-offsetX, paqHeight / 2), // Medio izquierda
			  Vector2D(paqWidth + offsetX, paqHeight / 2), // Medio derecha
			  Vector2D(paqWidth / 2, paqHeight / 2) // Centro
		};

		int greenPointIndex; 
		//Determina el lugar del punto verde dependiendo de la ruta que le pasen
		switch (routeID) {
		case 0:
			greenPointIndex = 0; // Superior izquierda
			break;
		case 1:
			greenPointIndex = 0; // Superior izquierda
			break;
		case 2:
			greenPointIndex = 1; // Superior derecha
			break;
		case 3:
			greenPointIndex = 4; // Inferior izquierda
			break;	
		default:
			greenPointIndex = -1; // Significa que no se aplica ningún punto verde
			break;
		}

		for (int i = 0; i < redPoints.size(); ++i) {
			Texture* currentTex = (i == greenPointIndex) ? puntoVerdeTex : puntoRojoTex;

			ecs::Entity* puntoEnt = ent_->getMngr()->addEntity(ecs::layer::WRAP_POINTS);
			Transform* puntoTr = puntoEnt->addComponent<Transform>(
				redPoints[i].getX() - currentTex->width() * scale / 2,
				redPoints[i].getY() - currentTex->height() * scale / 2,
				currentTex->width() * scale,
				currentTex->height() * scale
				);
			puntoTr->setParent(paqTr);
			puntoEnt->addComponent<RenderImage>(currentTex);
		}

	}
	else {
		std::cout << "No se pudo cargar la textura del punto rojo" << std::endl;
	}
}
void Paquete::clearLayer(ecs::layer::layerId lyId) {
	// Obtener la escena a la que pertenece la entidad
	 ecs::Scene* scene_ = ent_->getMngr();

	// Eliminar todas las entidades de la capa WRAP_POINTS
	scene_->removeEntitiesByLayer(lyId);
}


void Paquete::drawLines(int routeID, std::string routeName) {

	Transform* paqTr = ent_->getComponent<Transform>();


	// Creamos la entidad para el punto rojo
	Texture* lineaRojaTex = &sdlutils().images().at(routeName);
	float scale = 0.95f;
	int lineaRojaPosX = -35;
	int lineaRojaPosY = -35;
	ecs::Entity* lineaRojaEnt = ent_->getMngr()->addEntity(ecs::layer::RED_LINES);
	Transform* lineaRojaTr = lineaRojaEnt->addComponent<Transform>(lineaRojaPosX, lineaRojaPosY,
		lineaRojaTex->width() * scale,
		lineaRojaTex->height() * scale);
	lineaRojaTr->setParent(paqTr); // Establecer el paquete como padre del punto rojo
	lineaRojaEnt->addComponent<RenderImage>(lineaRojaTex);
}



std::string Paquete::getDirecction()
{
	// vamos a hacer que ponga exterior / interior y luego codigo postal
	std::string dir;
	if (miDistrito_ < 4)
		dir = "Exterior - ";
	else
		dir = "Interior - ";

	//creacion de codigo postal
	//se puede mejorar el fallo si se hace que el codigo postal pase a ser un numero aleatorio
	if (miDistrito_ == Erroneo) {
		int rand = sdlutils().rand().nextInt(0, 7);
		dir += std::bitset<3>(rand).to_string() + "\n";
	}
	else
		dir += std::bitset<3>(miDistrito_ + 1).to_string() + "\n";

	dir += nombreCalle_;

	return dir;
}

void Paquete::giveData(std::ofstream& stream) const{
	stream << (int)miDistrito_ << "," << miRemitente_ << "," << (int)miCalle_<<"\n";
}

