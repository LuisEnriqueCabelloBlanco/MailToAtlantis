#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <sistemas/NPC.h>
#include <sistemas/NPCevent.h>
using namespace npc;
npc::NPCMenorData::~NPCMenorData()
{

}
// NPC MENOR
NPCMenorData::NPCMenorData(Felicidad Felicidad, std::vector<bool>& DiasDanEvento):NPCdata(Campesino,5) {
	felicidad = Felicidad;
	postConversation = false;
	numMisionesAceptadas = 0;
	giveEvent = false;
	diasDanEvento = DiasDanEvento;
	eventosCompletados = std::vector<std::pair<bool,int>>(5,std::make_pair(false,0));
	iteration = 1;
}

npc::NPCMenorData::NPCMenorData(Personaje charac, JSONObject& obj):NPCdata(charac,5)
{
	events = std::vector<NPCevent*>(5, nullptr);
	JSONObject jDiasEvento = obj["DiasConEvento"]->AsObject();

	// leemos los 14 booleanos que indican los dias de evento
	for (int j = 0; j < 14; j++)
	{
		diasDanEvento.push_back(jDiasEvento[std::to_string(j + 1)]->AsBool());
	}
	iteration = 1;
	giveEvent = false;
	introText = obj["IntroductionText"]->AsString();
}

std::pair<const std::string, int> NPCMenorData::getDialogueInfo() {

	std::string tipo;
	int iterationNum = -1;

	if (felicidad == Minima || (felicidad == Maxima && !gD().getUpgradeValue(npcId)) || felicidad == NoHabladoAun)
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
			gD().unlockUpgrade(npcId);
			break;
		}
	}
	else if (giveEvent)
	{
		misionAceptada = true;
		tipo = "Eventos";
		iterationNum = numMisionesAceptadas + 1;
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
	int day = gD().getDay() - 1;
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

	if (lastIterationResult == -1) {
		iteration = sdlutils().rand().nextInt(1, 4);

		if (iteration > 3)
			iteration = 1;

		lastIterationResult = iteration;
	}

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

NPCMayorData::NPCMayorData(Felicidad Felicidad) :NPCdata(Vagabundo,14){
	felicidad = Felicidad;
	postConversation = false;
	numMisionesAceptadas = 0;
	if (npcId == Vagabundo) {
		firstMision = 1;
		diaDaMejora = 7;
	}
	else {
		firstMision = 5;
		diaDaMejora = 10;
	}
		
	misionAceptada = false;
	eventosCompletados = std::vector<std::pair<bool, int>>(14, std::make_pair(false, 0));
}

npc::NPCMayorData::NPCMayorData(Personaje charId, JSONObject& charRoot):NPCdata(charId, 14)
{
	events = std::vector<NPCevent*>(14, nullptr);
	firstMision = charRoot["PrimeraMision"]->AsNumber();
	introText = charRoot["IntroductionText"]->AsString();
}

std::pair<const std::string, int> NPCMayorData::getDialogueInfo() {
	std::string aux;

	if (!gD().getUpgradeValue(npcId) && gD().getDay() == diaDaMejora)
	{
		gD().unlockUpgrade(npcId);
	}
	else
	{
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
		case SeFue:
			aux = "FelicidadMinima";
			break;
		default:
			misionAceptada = true;
			aux = postConversation ?
				"PostConversacionDia" : "Dia";
			aux = aux + std::to_string(gD().getDay());
			break;
		}
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
	events[firstMision + numMisionesAceptadas]->numEvento = numMisionesAceptadas;
	return events[firstMision + numMisionesAceptadas];
}

npc::NPCdata::NPCdata(Personaje charac, int evtAmount)
{
	npcId = charac;
	felicidad = NoHabladoAun;
	numFelicidad = 50;
	postConversation = false;
	numMisionesAceptadas = 0;
	misionAceptada = false;
	eventosCompletados = std::vector<std::pair<bool, int>>(evtAmount, std::make_pair(false, 0));
}

npc::NPCdata::~NPCdata()
{
	for (auto ev : events) {
		delete ev;
	}
}

void npc::NPCdata::reset()
{
	felicidad = NoHabladoAun;
	numFelicidad = 50;
	postConversation = false;
	numMisionesAceptadas = 0;
	misionAceptada = false;
	eventosCompletados = std::vector<std::pair<bool, int>>(eventosCompletados.size(), std::make_pair(false, 0));
}

void npc::NPCdata::loadDataFromSaveFile(JSONObject& obj)
{
	felicidad = gD().stringToFelicidad(obj["Felicidad"]->AsString());
	postConversation = false;
	numMisionesAceptadas = obj["numMisionesAceptadas"]->AsNumber();
	numFelicidad = obj["FelicidadNum"]->AsNumber();
	JSONArray events = obj["EventosCompletados"]->AsArray();
	int k = 0;
	for (auto it : events)
	{
		eventosCompletados[k].first = it->AsNumber() > 0;
		eventosCompletados[k].second = it->AsNumber();
		k++;
	}
}
