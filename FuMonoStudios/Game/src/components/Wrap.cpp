
#include "Wrap.h"
#include "Transform.h"
#include "MultipleTextures.h"

#include "../architecture/Entity.h"
#include "../sdlutils/InputHandler.h"
#include "../json/JSON.h"
#include "../sdlutils/Texture.h"
#include "../sdlutils/SDLUtils.h"

#include <SDL.h>
#include <vector>
#include <assert.h>
#include <cmath>

//Constructora dada una ruta especifica
Wrap::Wrap(float spaceAux, int repTimesAux, std::list<int> routeAux) : space(spaceAux), repTimes(repTimesAux) {

	route = routeAux;

	totalPointsRoute = route.size() * (repTimes + 1);



}

//constructora con ruta base pero pudiendo escoger radio de deteccion de puntos y numero de repeticiones de patron
Wrap::Wrap(float spaceAux, int repTimesAux) : space(spaceAux), repTimes(repTimesAux) {

	route.push_back(pointRoute::LeftUp);

	route.push_back(pointRoute::LeftDown);

	route.push_back(pointRoute::RightDown);

	route.push_back(pointRoute::RightUp);

	totalPointsRoute = route.size() * (repTimes + 1);

}

//Ruta base y no se repite el patron de este
Wrap::Wrap(float spaceAux) : space(spaceAux) {

	route.push_back(pointRoute::LeftUp);

	route.push_back(pointRoute::LeftDown);

	route.push_back(pointRoute::RightDown);

	route.push_back(pointRoute::RightUp);

	totalPointsRoute = route.size() * (repTimes + 1);

}

Wrap::~Wrap() {


}

void Wrap::initComponent() {

	tr_ = ent_->getComponent<Transform>();

	tri_ = ent_->getComponent<Trigger>();

	mul_ = ent_->getComponent<RenderImage>();

	restartRoute();


	assert(tr_ != nullptr);

	assert(tri_ != nullptr);

	assert(mul_ != nullptr);

}

void Wrap::update() {

	//Primero se revisa si el paquete aun no se ha envuelto
	if (!wrapped) {
		auto& ihdlr = ih();

		SDL_Point point{ ihdlr.getMousePos().first, ihdlr.getMousePos().second };


		Vector2D posTR = tr_->getPos();

	
		//Se inicializan las posiciones del paquete para no acceder al transform y coordenadas constantemente
		double posXTR = posTR.getX();

		double posYTR = posTR.getY();

		float widthTR = tr_->getWidth();

		float heightTR = tr_->getHeigth();
		
		auto tapeEnt = tri_->getSpecificEntity(ecs::layer::TAPE);



		//primero se comprueba que la caja este tocando con la cinta
		if (tapeEnt != nullptr) {

			//despues se comprueba que la cinta esta tocando con el rat�n
			//a futuro si se ve un bug se podria a�adir que adem�s el rat�n este pulsado pero no deber�a hacer falta
			SDL_Rect tapeRect = tapeEnt->getComponent<Transform>()->getRect();

			if (SDL_PointInRect(&point, &tapeRect)) {


	
				// Se calculan los puntos centrales tanto del paquete como de la cinta
				double centerXTR = posXTR + widthTR / 2;
				double centerYTR = posYTR + heightTR / 2;

				double centerXTape = tapeRect.x + tapeRect.w / 2;
				double centerYTape = tapeRect.y + tapeRect.h / 2;

				drawRedPoints();
	

				//SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
				//SDL_RenderDrawLine(renderer, centerXTR, centerYTR, centerXTR + 200, centerYTR + 200);

				//
				//Vector2D size{ puntoRojoTex->width() * 0.5f, puntoRojoTex->height() * 0.5f };
				//Vector2D puntoRojoPos(centerPos.getX() - puntoRojoTex->width() * 0.25f, centerPos.getY() - puntoRojoTex->height() * 0.25f);

				//// Renderiza la textura de punto rojo en las coordenadas calculadas
				//puntoRojoTex->render(puntoRojoPos.getX(), puntoRojoPos.getY(), nullptr, 0.0, nullptr, SDL_FLIP_NONE);

				std::vector<double> pointsX{ abs(centerXTape - (centerXTR - widthTR / 2)), abs(centerXTape - centerXTR), abs(centerXTape - (centerXTR + widthTR / 2)) };

				std::vector<double> pointsY{ abs(centerYTape - (centerYTR - heightTR / 2)), abs(centerYTape - centerYTR), abs(centerYTape - (centerYTR + heightTR / 2)) };
				int nCheckPointRoute = 0;

				for (int i = 0; i < pointsY.size(); ++i) {

					for (int j = 0; j < pointsX.size(); ++j) {

						if (debug && false) {
							std::cout << nCheckPointRoute << std::endl;
						}

						if (pointsX[j] < space && pointsY[i] < space) {

							checkPointTouch(nCheckPointRoute);

						}

						nCheckPointRoute++;

					}
				}


				//Se comprueba el % de paquete empaquetado para hacer el cambio de sprites
				if ((routePointsDone == totalPointsRoute / 4 && wrapFase < 1)
					|| (routePointsDone == totalPointsRoute / 2 && wrapFase < 2)
					|| (routePointsDone == (totalPointsRoute * 3) / 4 && wrapFase < 3)
					|| (routePointsDone == totalPointsRoute && wrapFase < 4)) {

					mul_->nextTexture();
					wrapFase++;

				}

				//Si se ha recorrido toda la ruta se comprueba si quedan repeticiones de patron. En caso contrario se da por envalado
				if (lastPoint == route.end()) {

					if (debug) {
						std::cout << "Embalado" << std::endl;
					}

					repTimes--;

					if (repTimes < 0) {

						wrapped = true;

					}

					restartRoute();
				}			

			}

		}
		else {
			restartRoute();
		}
		
	}
	

}

//Reinicia la ruta de envalado
void Wrap::restartRoute() {

	lastPoint = route.begin();

	beforePoint = route.begin();

	routePointsDone = 0;

	wrapFase = 0;

	if (!wrapped) {
		mul_->setTexture(0);
	}
	

}


void Wrap::checkPointTouch(int point) {

	if ((*lastPoint) == point) {
		std::cout << point << "Correct" << std::endl;

		if (lastPoint != route.begin()) {
			++beforePoint;
		}

		++lastPoint;
		routePointsDone++;
	}
	else if (point != (*beforePoint)) {

		restartRoute();

	}
}

void Wrap::drawRedPoints() {

		Paquete* paqComp = ent_->getComponent<Paquete>();
		paqComp->puntosRojos();
	
}

//
//void Wrap::drawLines() {
//	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE); // Color blanco
//	for (const auto& line : linesDrawn) {
//		SDL_RenderDrawLine(renderer, line.start.x, line.start.y, line.end.x, line.end.y);
//	}
//}
//
//void Wrap::addLine(const SDL_Point& start, const SDL_Point& end) {
//	Line line{ start, end };
//	linesDrawn.push_back(line);
//}


