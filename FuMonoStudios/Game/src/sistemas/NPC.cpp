#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <sistemas/NPC.h>
#include <sistemas/NPCevent.h>
#include <architecture/GameConstants.h>
using namespace npc;
npc::NPCMenorData::~NPCMenorData()
{

}
// NPC MENOR

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

	if (felicidad == Minima || felicidad == NoHabladoAun)
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
		}
	}
	else if (numFelicidad >= UNLOCK_UPGRADE_HAPPINES && !unlockUpgrade) {

		unlockUpgrade = true;
		tipo = "FelicidadMaxima";
		gD().unlockUpgrade(npcId);

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
		case Maxima:
			tipo = "GenericoMaxiBueno";
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
	misionAceptada = false;
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

npc::NPCMayorData::NPCMayorData(Personaje charId, JSONObject& charRoot):NPCdata(charId, 14)
{
	events = std::vector<NPCevent*>(14, nullptr);
	firstMision = charRoot["PrimeraMision"]->AsNumber();
	introText = charRoot["IntroductionText"]->AsString();
	diaDaMejora = charRoot["DiaMejora"]->AsNumber();
}

std::pair<const std::string, int> NPCMayorData::getDialogueInfo() {
	std::string aux;

	if (!gD().getUpgradeValue(npcId) && gD().getDay() == diaDaMejora)
	{
		gD().unlockUpgrade(npcId);
	}

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

	return std::make_pair(aux, -1);
}

void NPCMayorData::setupDayData() {
	if (postConversation && felicidad == NoHabladoAun)
		felicidad = Normal;
	postConversation = false;
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
	unlockUpgrade = false;
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
	unlockUpgrade = false;
	eventosCompletados = std::vector<std::pair<bool, int>>(eventosCompletados.size(), std::make_pair(false, 0));
}

void npc::NPCdata::loadDataFromSaveFile(JSONObject& obj)
{
	felicidad = gD().stringToFelicidad(obj["Felicidad"]->AsString());
	postConversation = false;
	numMisionesAceptadas = obj["numMisionesAceptadas"]->AsNumber();
	numFelicidad = obj["FelicidadNum"]->AsNumber();
	unlockUpgrade = obj["unlockUpgrade"]->AsBool();
	JSONArray events = obj["EventosCompletados"]->AsArray();
	int k = 0;
	for (auto it : events)
	{
		eventosCompletados[k].first = it->AsNumber() > 0;
		eventosCompletados[k].second = it->AsNumber();
		k++;
	}
}
