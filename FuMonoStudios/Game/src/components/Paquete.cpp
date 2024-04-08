
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
	return calleMarcada_ != Erronea && calleMarcada_ == miCalle_;
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
		pesoCorrecto()&&
		correctFragile()
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
		ecs::Entity* selloEnt = ent_->getMngr()->addEntity(ecs::layer::STAMP);
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

void Paquete::puntosRojos() {
	Transform* paqTr = ent_->getComponent<Transform>();

	// Creamos la entidad para el punto rojo
	Texture* puntoRojoTex = &sdlutils().images().at("puntoRojo");
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
			// Esquinas
			  Vector2D(-offsetX, -offsetY), // Superior izquierda
			  Vector2D(paqWidth + offsetX, -offsetY), // Superior derecha
			  Vector2D(-offsetX, paqHeight + offsetY), // Inferior izquierda
			  Vector2D(paqWidth + offsetX, paqHeight + offsetY), // Inferior derecha
			  // Mitad entre las esquinas y el centro
			  Vector2D(paqWidth / 2, -offsetY), // Medio superior
			  Vector2D(paqWidth / 2, paqHeight + offsetY), // Medio inferior
			  Vector2D(-offsetX, paqHeight / 2), // Medio izquierda
			  Vector2D(paqWidth + offsetX, paqHeight / 2), // Medio derecha
			  // Centro
			  Vector2D(paqWidth / 2, paqHeight / 2) // Centro
		};

		// Agregar un punto rojo en cada posición calculada
		for (const auto& pos : redPoints) {
			ecs::Entity* puntoRojoEnt = ent_->getMngr()->addEntity(ecs::layer::WRAP_POINTS);
			Transform* puntoRojoTr = puntoRojoEnt->addComponent<Transform>(
				pos.getX() - puntoRojoTex->width() * scale / 2,
				pos.getY() - puntoRojoTex->height() * scale / 2,
				puntoRojoTex->width() * scale,
				puntoRojoTex->height() * scale
				);
			puntoRojoTr->setParent(paqTr); // Establecer el paquete como padre del punto rojo
			puntoRojoEnt->addComponent<RenderImage>(puntoRojoTex);
		}
	}
	else {
		std::cout << "No se pudo cargar la textura del punto rojo" << std::endl;
	}
}
void Paquete::eliminarPuntosRojos() {
	// Obtener la escena a la que pertenece la entidad
	 ecs::Scene* scene_ = ent_->getMngr();

	// Eliminar todas las entidades de la capa WRAP_POINTS
	scene_->removeEntitiesByLayer(ecs::layer::WRAP_POINTS);
}


void Paquete::drawLines() {

	Transform* paqTr = ent_->getComponent<Transform>();

	auto& ihdlr = ih();

	SDL_Point point{ ihdlr.getMousePos().first, ihdlr.getMousePos().second };

	// Creamos la entidad para el punto rojo
	Texture* lineaRojaTex = &sdlutils().images().at("lineaRoja");
	float scale = 0.2f;

	ecs::Entity* lineaRojaEnt = ent_->getMngr()->addEntity(ecs::layer::RED_LINES);
	Transform* lineaRojaTr = lineaRojaEnt->addComponent<Transform>(
		20,
		-40,
		lineaRojaTex->width() * scale + 20,
		lineaRojaTex->height() * scale
	);
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

