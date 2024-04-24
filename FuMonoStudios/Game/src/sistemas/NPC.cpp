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
	postConversation = false;
	eventosCompletados = std::vector<bool>(5,false);
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
		iterationNum = selectedEvent.first + 1;
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
	postConversation = false;
	iteration = 1;
	giveEvent = diasDanEvento[generalData().getDay() - 1];

	// comprobar si hemos completado todos los eventos
	bool npcCompleted = true;
	int i = 0;
	while (npcCompleted && i < eventosCompletados.size())
	{
		npcCompleted = eventosCompletados[i];
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
			if (!eventosCompletados[i])
				eventosNoCompletados.push_back(i);
		}

		int seleccion = eventosNoCompletados[sdlutils().rand().nextInt(0, eventosCompletados.size())];
		selectedEvent.first = seleccion;
		selectedEvent.second = events[seleccion];
	}
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
	numMisionesAceptadas++;
	return selectedEvent.second;
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
		postConversation = true;
		break;
	case Minima:
		aux = "FelicidadMinimo";
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
	postConversation = false;
}

NPCevent* npc::NPCMayorData::getEvent()
{
	if (postConversation)
		return nullptr;

	postConversation = true;
	numMisionesAceptadas++;
	return events[numMisionesAceptadas];
}

