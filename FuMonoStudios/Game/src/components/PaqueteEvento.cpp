#include "PaqueteEvento.h"
#include "../architecture/GeneralData.h"

PaqueteEvento::PaqueteEvento(GeneralData::NPCevent* event) {
	event_ = event;
}

PaqueteEvento::~PaqueteEvento() {

}

void PaqueteEvento::paqueteSent(Distrito tubo) {

}