#include "CristalBall.h"

CristalBall::CristalBall(RenderImage* r) : showing(0), myRender(r){	
	myRender->nextTexture();
}
CristalBall::~CristalBall() {

}
void CristalBall::check(Paquete*p, bool goNeutral) {
	if (goNeutral) {
		if(showing == 1) myRender->previousTexture();
		if (showing == -1) myRender->nextTexture();

		showing = 0;
	}
	else {
		if (p->correcto()) {			
			if (showing == -1) myRender->nextTexture(); //Por si acaso la bola estaba en la textura de errónea
			if(showing != 1) myRender->nextTexture();
			showing = 1;
		}
		else {			
			if (showing == 1) myRender->previousTexture(); //Por si acaso la bola estaba en la textura de correcta			
			if (showing != -1) myRender->previousTexture();		
			showing = -1;
		}
	}
}