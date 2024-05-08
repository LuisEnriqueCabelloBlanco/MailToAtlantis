#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <sistemas/NPC.h>
#include <architecture/GeneralData.h>
#include <sdlutils/SDLUtils.h>
#include <sistemas/NPCevent.h>
using namespace npc;
npc::NPCMenorData::~NPCMenorData()
{

}
// NPC MENOR
NPCMenorData::NPCMenorData(Felicidad Felicidad, std::vector<bool> DiasDanEvento) {
	felicidad = Felicidad;
	iteration = 1;
	diasDanEvento = DiasDanEvento;
	giveEvent = false;
	postConversation = false;
	eventosCompletados = std::vector<std::pair<bool,int>>(5,std::make_pair(false,0));
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
			postConversation = true;
			break;
		case Minima:
			tipo = "FelicidadMinima";
			felicidad = SeFue;
			break;
		case Maxima:
			tipo = "FelicidadMaxima";
			break;
		}
	}
	else if (giveEvent)
	{
		tipo = "Eventos";
		iterationNum = numMisionesAceptadas;
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
	if (postConversation && felicidad == NoHabladoAun)
		felicidad = Normal;
	postConversation = false;
	iteration = 1;
	int day = generalData().getDay() - 1;
	giveEvent = diasDanEvento[day];
	if (misionAceptada) {
		numMisionesAceptadas++;
	}
	misionAceptada = false;
	/*// comprobar si hemos completado todos los eventos
	bool npcCompleted = true;
	int i = 0;
	while (npcCompleted && i < eventosCompletados.size())
	{
		npcCompleted = eventosCompletados[i].first;
		i++;
	}
	if (npcCompleted)
		giveEvent = false;

	selectedEvent.second = nullptr;
	// seleccionar evento si es dia de dar evento
	if (giveEvent)
	{
		std::vector<int> eventosNoCompletados;
		for (int i = 0; i < eventosCompletados.size(); i++)
		{
			if (!eventosCompletados[i].first)
				eventosNoCompletados.push_back(i);
		}

		int seleccion = eventosNoCompletados[sdlutils().rand().nextInt(0, eventosCompletados.size())];
		selectedEvent.first = seleccion;
		selectedEvent.second = events[seleccion];
	}*/
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
	if (postConversation)
		return nullptr;

	postConversation = true;
	return events[numMisionesAceptadas];
}

// NPC GRANDE

npc::NPCMayorData::~NPCMayorData()
{
}

NPCMayorData::NPCMayorData(Felicidad Felicidad) {
	felicidad = Felicidad;
	postConversation = false;
	eventosCompletados = std::vector<std::pair<bool, int>>(14, std::make_pair(false, 0));
}

std::pair<const std::string, int> NPCMayorData::getDialogueInfo() {
	std::string aux;

	switch (felicidad)
	{
	case NoHabladoAun:
		aux = "Presentacion";
		postConversation = true;
		break;
	case Minima:
		aux = "FelicidadMinima";
		felicidad = SeFue;
		break;
	case Maxima:
		aux = "FelicidadMaxima";
		break;
	default:
		aux = postConversation ?
			"PostConversacionDia" : "Dia";
		aux = aux + std::to_string(generalData().getDay());
		break;
	}

	return std::make_pair(aux, -1);
}

void NPCMayorData::setupDayData() {
	if (postConversation && felicidad == NoHabladoAun)
		felicidad = Normal;
	postConversation = false;
	if (misionAceptada) {
		numMisionesAceptadas++;
	}
	misionAceptada = false;
}

NPCevent* npc::NPCMayorData::getEvent()
{
	if (postConversation)
		return nullptr;

	postConversation = true;
	return events[numMisionesAceptadas];
}

npc::NPCdata::~NPCdata()
{
	for (auto ev : events) {
		delete ev;
	}
}
