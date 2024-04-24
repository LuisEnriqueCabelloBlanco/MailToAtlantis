#include <sistemas/NPC.h>
#include <architecture/GeneralData.h>
#include <sdlutils/SDLUtils.h>
using namespace npc;
// NPC MENOR
NPCMenorData::NPCMenorData(Felicidad Felicidad, std::vector<bool> DiasDanEvento) {
	felicidad = Felicidad;
	iteration = 1;
	diasDanEvento = DiasDanEvento;
	giveEvent = false;
}

std::pair<const std::string, int> NPCMenorData::getDialogueInfo() {

	std::string tipo;
	int iterationNum = -1;

	if (felicidad == Minima || felicidad == Maxima || felicidad == NoHabladoAun)
	{
		switch (felicidad)
		{
		case NoHabladoAun:
			tipo = "Presentacion";
			felicidad = Normal;
			break;
		case Minima:
			tipo = "FelicidadMinimo";
			break;
		case Maxima:
			tipo = "FelicidadMaximo";
			break;
		}
	}
	else if (giveEvent)
	{
		tipo = "Eventos";
		iterationNum = sdlutils().rand().nextInt(1, 6);
	}
	else
	{
		switch (felicidad) {
		case Mala:
			tipo = "GenericoMalo";
			iterateDialogues();
			iterationNum = iteration;
			break;
		case Normal:
			tipo = "GenericoNormal";
			iterateDialogues();
			iterationNum = iteration;
			break;
		case Buena:
			tipo = "GenericoBueno";
			iterateDialogues();
			iterationNum = iteration;
			break;
		}
	}
	return std::make_pair(tipo, iterationNum);
}

void NPCMenorData::setupDayData() {
	iteration = 1;
	giveEvent = diasDanEvento[generalData().getDay() - 1];
}

void NPCMenorData::activateEvent() {
	giveEvent = true;
}

void NPCMenorData::deactivateEvent() {
	giveEvent = false;
}

void NPCMenorData::iterateDialogues() {
	iteration++;
	if (iteration > 3)
		iteration = 1;
}

NPCevent* NPCMenorData::getEvent() {
	return events[sdlutils().rand().nextInt(0, 5)];
	numMisionesAceptadas++;
}

// NPC GRANDE

NPCMayorData::NPCMayorData(Felicidad Felicidad) {
	felicidad = Felicidad;
	postConversation = false;
}

std::pair<const std::string, int> NPCMayorData::getDialogueInfo() {
	std::string aux;

	switch (felicidad)
	{
	case NoHabladoAun:
		aux = "Presentacion";
		felicidad = Normal;
		break;
	case Minima:
		aux = "FelicidadMinimo";
		break;
	default:
		aux = postConversation ?
			"PostConversacionDia" : "Dia";
		aux = aux + std::to_string(generalData().getDay());
		postConversation = true;
		break;
	}

	return std::make_pair(aux, -1);
}

void NPCMayorData::setupDayData() {
	postConversation = false;
}

NPCevent* npc::NPCMayorData::getEvent()
{
	numMisionesAceptadas++;
	return events[numMisionesAceptadas];
}

