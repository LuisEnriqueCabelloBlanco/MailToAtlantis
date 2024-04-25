#include "NPCeventSystem.h"
#include "../json/JSON.h"
#include <random>
#include <algorithm>
#include "../architecture/GeneralData.h"
#include "../sistemas/PaqueteBuilder.h"
#include "../architecture/Game.h"

NPCeventSystem::NPCeventSystem() {
	readNPCEventData();
}

NPCeventSystem::~NPCeventSystem() {
	for (auto it : activeEventsNPCs) {
		delete it;
	}
	for (auto it : paquetesNPCs) {
		
	}
}

Paquete* NPCeventSystem::getPaqueteNPC() {
	Paquete* p = paquetesNPCs.back();
	paquetesNPCs.pop_back();
	return p;
}

void NPCeventSystem::shuffleNPCqueue() {
	std::random_device rd;  // Obtener una semilla aleatoria desde el dispositivo
	std::mt19937 seed(rd());   // Generador de números aleatorios

	std::shuffle(paquetesNPCs.begin(), paquetesNPCs.end(), seed);
}


bool NPCeventSystem::areTherePaquetesNPC() {
	return paquetesNPCs.size() != 0;
}

void NPCeventSystem::checkPaqueteSent(Paquete* p) {
	for (NPCevent* event : activeEventsNPCs) {

		for (auto conditionVec : event->condiciones) {
			bool valid = true;

			int i = 0;
			while (valid && i < conditionVec.size())
			{
				valid = conditionVec[i](p);
				i++;
			}

			if (valid && i >= conditionVec.size() && conditionVec.size() > 0)
			{
				event->paqueteSuccesful();
			}
		}
	}
}

void NPCeventSystem::minigameOver() {

	for (NPCevent* event : activeEventsNPCs)
	{
		std::cout << "Event " << (event->completed ? "completed" : "failed");
		NPCdata* data = generalData().getNPCData(event->personaje);
		data->eventosCompletados[event->numEvento].first = true;
		data->eventosCompletados[event->numEvento].second = 
			generalData().getDay() * event->completed ? 1 : -1;
		if (event->completed)
			procesarStringRecompensas(event->recompensas);
	}

	while (areTherePaquetesNPC())
		paquetesNPCs.pop_back();
}

void NPCeventSystem::addPaqueteNPC(Paquete* p) {
	paquetesNPCs.push_back(p);
}

void NPCeventSystem::activateEvent(NPCevent* e) {
	activeEventsNPCs.push_back(e);
}

void NPCeventSystem::procesarStringRecompensas(std::vector<std::string> vec) {

	for (std::string& reward : vec) {
		// si tiene un sumar o restar
		if (reward.find("+") != std::string::npos || reward.find("-") != std::string::npos)
		{
			int index = reward.find_first_of("+-");
			std::string personajeString = reward.substr(0, index);
			int felicidadIncrement = reward.size() - index;
			if (reward.find("-") != std::string::npos)
				felicidadIncrement = -felicidadIncrement;

			npc::Personaje aux = generalData().stringToPersonaje(personajeString);

			generalData().incrementarFelicidad(aux, felicidadIncrement);
		}
		else if (reward.find("$") != std::string::npos)
		{
			int index = reward.find_first_of("$");

			std::string personajeString = reward.substr(index + 1, reward.size());

			npc::Personaje aux = generalData().stringToPersonaje(personajeString);

			generalData().unlockMejoraPersonaje(aux);
		}
	}
}

void NPCeventSystem::debugPaquetesInQueue() {
	std::cout << std::endl << "Eventos: " << activeEventsNPCs.size() << 
		" Paquetes de npc: " << paquetesNPCs.size() << std::endl;
}


void NPCeventSystem::readPaquetes(JSONObject obj, NPCevent* auxEvent) {

	for (int i = 0; i < auxEvent->numPaquetes; i++)
	{
		PaqueteBuilder paqBuild(gm().getScene(ecs::sc::MAIN_SCENE));

		bool legal = true;

		std::string rem = paqBuild.remitenteRND();
		auto hasRemitente = obj.find("remitente");
		if (hasRemitente != obj.end())
			rem = hasRemitente->second->AsString();

		Distrito dist = (Distrito)sdlutils().rand().nextInt(0, generalData().getTubesAmount());
		auto hasDistrito = obj.find("distrito");
		if (hasDistrito != obj.end())
		{

			dist = (Distrito)generalData().fromStringToDistrito(
				hasDistrito->second->AsString());

			if (legal && dist == Erroneo)
				legal = false;
		}

		Calle calle = (Calle)sdlutils().rand().nextInt(0, 3);
		auto hasCalle = obj.find("calle");
		if (hasCalle != obj.end())
		{
			calle = generalData().stringToCalle(hasCalle->second->AsString());

			if (legal && calle == Erronea)
				legal = false;
		}

		TipoPaquete tipo = (TipoPaquete)sdlutils().rand().nextInt(0, 5);
		auto hasTipo = obj.find("tipoPaquete");
		if (hasTipo != obj.end())
		{
			tipo = generalData().stringToTipoPaquete(hasTipo->second->AsString());
		}

		int peso = 0;
		NivelPeso nivelPeso = Ninguno;
		bool fragil = false;

		if (generalData().getPaqueteLevel() > 1)
		{
			fragil = sdlutils().rand().nextInt(0, 10);
			fragil = fragil < 2;
			auto hasFragil = obj.find("fragil");
			if (hasFragil != obj.end())
				fragil = hasFragil->second->AsBool();

			if (generalData().getPaqueteLevel() > 2)
			{
				// esto toma los valores brutamente del json
				nivelPeso = paqBuild.pesoRND(20, 30, peso);
				auto hasNivelPeso = obj.find("peso");
				if (hasNivelPeso != obj.end())
				{
					nivelPeso = generalData().stringToNivelPeso(hasNivelPeso->second->AsString());
				}

				auto hasPesoKG = obj.find("pesoKG");
				if (hasPesoKG != obj.end())
					peso = hasPesoKG->second->AsNumber();

			}
		}

		auto hasLegal = obj.find("legal");
		if (hasLegal != obj.end())
			legal = hasLegal->second->AsBool();

		std::string nombreCalle;
		if (calle == Erronea || dist == Erroneo)
		{
			//Cambiarlo por el sistema de calles err�neas una vez est�
			//Simplemente ser�a meterlas en el mismo json, en el distrito erroneo y modificar el getStreetsFromJson
			//Y meterle un randomizador para que de esas pille la que m�s le guste
			//Tipo, haces distritoCalle_[Erroneo][rand]
			//dir = "(CALLE INVENTADA)";
			int rnd = sdlutils().rand().nextInt(0, paqBuild.distritoCalle_[Erroneo].size());
			nombreCalle = paqBuild.distritoCalle_[Erroneo][rnd];
		}
		else
			nombreCalle = paqBuild.distritoCalle_[dist][(int)calle];

		Paquete* paquete = new Paquete(dist, calle, nombreCalle, rem, tipo, legal,
			nivelPeso, peso, fragil);

		auxEvent->paquetes.push_back(paquete);
	}

}

void NPCeventSystem::readPaquetesEspecificos(JSONObject obj, NPCevent* auxEvent) {
	for (auto paq : obj) {
		JSONObject paqObj = paq.second->AsObject();

		PaqueteBuilder paqBuild(gm().getScene(ecs::sc::MAIN_SCENE));

		bool legal = true;

		std::string rem = paqBuild.remitenteRND();
		auto hasRemitente = paqObj.find("remitente");
		if (hasRemitente != paqObj.end())
			rem = hasRemitente->second->AsString();

		Distrito dist = (Distrito)sdlutils().rand().nextInt(0, generalData().getTubesAmount());
		auto hasDistrito = paqObj.find("distrito");
		if (hasDistrito != paqObj.end())
		{

			dist = (Distrito)generalData().fromStringToDistrito(
				hasDistrito->second->AsString());

			if (legal && dist == Erroneo)
				legal = false;
		}

		Calle calle = (Calle)sdlutils().rand().nextInt(0, 3);
		auto hasCalle = paqObj.find("calle");
		if (hasCalle != paqObj.end())
		{
			calle = generalData().stringToCalle(hasCalle->second->AsString());

			if (legal && calle == Erronea)
				legal = false;
		}

		TipoPaquete tipo = (TipoPaquete)sdlutils().rand().nextInt(0, 5);
		auto hasTipo = paqObj.find("tipoPaquete");
		if (hasTipo != paqObj.end())
		{
			tipo = generalData().stringToTipoPaquete(hasTipo->second->AsString());
		}

		int peso = 0;
		NivelPeso nivelPeso = Ninguno;
		bool fragil = false;

		if (generalData().getPaqueteLevel() > 1)
		{
			fragil = sdlutils().rand().nextInt(0, 10);
			fragil = fragil < 2;
			auto hasFragil = paqObj.find("fragil");
			if (hasFragil != paqObj.end())
				fragil = hasFragil->second->AsBool();

			if (generalData().getPaqueteLevel() > 2)
			{
				// esto toma los valores brutamente del json
				nivelPeso = paqBuild.pesoRND(20, 30, peso);
				auto hasNivelPeso = paqObj.find("peso");
				if (hasNivelPeso != paqObj.end())
				{
					nivelPeso = generalData().stringToNivelPeso(hasNivelPeso->second->AsString());
				}

				auto hasPesoKG = paqObj.find("pesoKG");
				if (hasPesoKG != paqObj.end())
					peso = hasPesoKG->second->AsNumber();

			}
		}

		auto hasLegal = paqObj.find("legal");
		if (hasLegal != paqObj.end())
			legal = hasLegal->second->AsBool();

		std::string nombreCalle;
		if (calle == Erronea || dist == Erroneo)
		{
			//Cambiarlo por el sistema de calles err�neas una vez est�
			//Simplemente ser�a meterlas en el mismo json, en el distrito erroneo y modificar el getStreetsFromJson
			//Y meterle un randomizador para que de esas pille la que m�s le guste
			//Tipo, haces distritoCalle_[Erroneo][rand]
			//dir = "(CALLE INVENTADA)";
			int rnd = sdlutils().rand().nextInt(0, paqBuild.distritoCalle_[Erroneo].size());
			nombreCalle = paqBuild.distritoCalle_[Erroneo][rnd];
		}
		else
			nombreCalle = paqBuild.distritoCalle_[dist][(int)calle];

		Paquete* paquete = new Paquete(dist, calle, nombreCalle, rem, tipo, legal,
			nivelPeso, peso, fragil);

		auxEvent->paquetes.push_back(paquete);
	}
}

void NPCeventSystem::readCondiciones(JSONObject obj, NPCevent* auxEvent) {
	std::vector<Condition> condicionesDeTodos;

	auto hasRemitente = obj.find("remitente");
	if (hasRemitente != obj.end()) {
		std::string aux = hasRemitente->second->AsString();
		condicionesDeTodos.push_back([aux](Paquete* p) -> bool {
			return p->getRemitente() == aux;
			});
	}

	auto hasDistrito = obj.find("distrito");
	if (hasDistrito != obj.end()) {

		Distrito aux = (Distrito)generalData().fromStringToDistrito(hasDistrito->second->AsString());
		condicionesDeTodos.push_back([aux](Paquete* p) -> bool {
			return p->getDistrito() == aux;
			});
	}

	auto hasCalle = obj.find("calleMarcada");
	if (hasCalle != obj.end()) {
		Calle aux = (Calle)generalData().stringToCalle(hasCalle->second->AsString());
		condicionesDeTodos.push_back([aux](Paquete* p) -> bool {
			return p->getCalleMarcada() == aux;
			});
	}

	auto hasTipo = obj.find("tipoPaquete");
	if (hasTipo != obj.end()) {
		TipoPaquete aux = generalData().stringToTipoPaquete(hasTipo->second->AsString());
		condicionesDeTodos.push_back([aux](Paquete* p) -> bool {
			return p->getTipo() == aux;
			});
	}

	auto hasPeso = obj.find("peso");
	if (hasPeso != obj.end()) {
		NivelPeso aux = generalData().stringToNivelPeso(hasPeso->second->AsString());
		condicionesDeTodos.push_back([aux](Paquete* p) -> bool {
			return p->getPeso() == aux;
			});
	}

	auto hasPesoKG = obj.find("pesoKG");
	if (hasPesoKG != obj.end()) {
		int aux = hasPesoKG->second->AsNumber();
		condicionesDeTodos.push_back([aux](Paquete* p) -> bool {
			return p->getCantidadPeso() == aux;
			});
	}

	auto hasLegal = obj.find("legal");
	if (hasLegal != obj.end()) {
		bool aux = hasLegal->second->AsBool();
		condicionesDeTodos.push_back([aux](Paquete* p) -> bool {
			return p->correcto() == aux;
			});
	}

	auto hasFragil = obj.find("fragil");
	if (hasFragil != obj.end()) {
		bool aux = hasFragil->second->AsBool();
		condicionesDeTodos.push_back([aux](Paquete* p) -> bool {
			return aux && p->getFragil() && p->getEnvuelto();
			});
	}

	auxEvent->condiciones.push_back(condicionesDeTodos);
}

void NPCeventSystem::readCondicionesEspecificos(JSONObject obj, NPCevent* auxEvent) {
	int i = 0;
	for (auto paq : obj)
	{
		auxEvent->condiciones.push_back(std::vector<Condition>(0));
		JSONObject pqConditions = paq.second->AsObject();

		auto hasRemitente = pqConditions.find("remitente");
		if (hasRemitente != pqConditions.end()) {
			std::string aux = hasRemitente->second->AsString();
			auxEvent->condiciones[i].push_back([aux](Paquete* p) -> bool {
				return p->getRemitente() == aux;
				});
		}

		auto hasDistrito = pqConditions.find("distrito");
		if (hasDistrito != pqConditions.end()) {

			Distrito aux = (Distrito)generalData().fromStringToDistrito(hasDistrito->second->AsString());
			auxEvent->condiciones[i].push_back([aux](Paquete* p) -> bool {
				return p->getDistrito() == aux;
				});
		}

		auto hasCalle = pqConditions.find("calleMarcada");
		if (hasCalle != pqConditions.end()) {
			Calle aux = (Calle)generalData().stringToCalle(hasCalle->second->AsString());
			auxEvent->condiciones[i].push_back([aux](Paquete* p) -> bool {
				return p->getCalleMarcada() == aux;
				});
		}

		auto hasTipo = pqConditions.find("tipoPaquete");
		if (hasTipo != pqConditions.end()) {
			TipoPaquete aux = generalData().stringToTipoPaquete(hasTipo->second->AsString());
			auxEvent->condiciones[i].push_back([aux](Paquete* p) -> bool {
				return p->getTipo() == aux;
				});
		}

		auto hasPeso = pqConditions.find("peso");
		if (hasPeso != pqConditions.end()) {
			NivelPeso aux = generalData().stringToNivelPeso(hasPeso->second->AsString());
			auxEvent->condiciones[i].push_back([aux](Paquete* p) -> bool {
				return p->getPeso() == aux;
				});
		}

		auto hasPesoKG = pqConditions.find("pesoKG");
		if (hasPesoKG != pqConditions.end()) {
			int aux = hasPesoKG->second->AsNumber();
			auxEvent->condiciones[i].push_back([aux](Paquete* p) -> bool {
				return p->getCantidadPeso() == aux;
				});
		}

		auto hasLegal = pqConditions.find("legal");
		if (hasLegal != pqConditions.end()) {
			bool aux = hasLegal->second->AsBool();
			auxEvent->condiciones[i].push_back([aux](Paquete* p) -> bool {
				return p->correcto() == aux;
				});
		}


		auto hasFragil = pqConditions.find("fragil");
		if (hasFragil != pqConditions.end()) {
			bool aux = hasFragil->second->AsBool();
			auxEvent->condiciones[i].push_back([aux](Paquete* p) -> bool {
				return aux && p->getFragil() && p->getEnvuelto();
				});
		}

		i++;
	}
}

void NPCeventSystem::readNPCevent(JSONObject eventObject, int personaje, int index) {
	NPCevent* auxEvent = new NPCevent();

	auxEvent->personaje = (npc::Personaje)personaje;
	auxEvent->numEvento = index;

	JSONObject currentEvent = eventObject.find(std::to_string(index + 1))->second->AsObject();
	auxEvent->numPaquetes = currentEvent.find("numPaquetes")->second->AsNumber();
	auxEvent->numPaquetesToComplete = currentEvent.find("numPaquetesParaCompletar")->second->AsNumber();
	auxEvent->textoDiario = currentEvent.find("textoDiario")->second->AsString();

	// Si es especial, nos saltamos el resto
	auto isSpecial = currentEvent.find("special");
	if (isSpecial != currentEvent.end())
	{
		// ES ESPECIAL
		int numSpecial = isSpecial->second->AsNumber();
		switch (numSpecial) {
		case 1:
			break;
		}
	}
	else
	{
		auto paquetesArray = currentEvent.find("paquetes");
		if (paquetesArray != currentEvent.end())
		{
			JSONObject obj = paquetesArray->second->AsObject();

			// SI ESPECIFICAMOS LOS DATOS PARA CADA PAQUETE 
			if (obj.find("1") != obj.end())
			{
				readPaquetesEspecificos(obj, auxEvent);
			}
			else // SI LOS DATOS SON PARA TODOS LOS PAQUETES
			{
				readPaquetes(obj, auxEvent);
			}
		}
		else
			throw std::runtime_error("Fallo cargando paquetes de evento");

		auto condicionesArray = currentEvent.find("condiciones");
		if (condicionesArray != currentEvent.end())
		{
			JSONObject obj = condicionesArray->second->AsObject();

			// SI ESPECIFICAMOS LAS CONDICIONES PARA CADA PAQUETE 
			if (obj.find("1") != obj.end())
			{
				readCondicionesEspecificos(obj, auxEvent);
			}
			else // SI LAS CONDICIONES SON PARA TODOS LOS PAQUETES
			{
				readCondiciones(obj, auxEvent);
			}
		}
		else
			throw std::runtime_error("Fallo cargando condiciones de evento");
	}
	auto hasRewards = currentEvent.find("recompensa");
	if (hasRewards != currentEvent.end())
	{
		// SI TENEMOS MAS DE UNA RECOMPENSA
		if (hasRewards->second->IsObject())
		{
			JSONObject obj = hasRewards->second->AsObject();
			for (auto rew : obj)
			{
				auxEvent->recompensas.push_back(rew.second->AsString());
			}
		}
		else // SI SOLO HAY UNA RECOMPENSA / NO HAY NINGUNA
		{
			if (hasRewards->second->AsString() != "ninguna")
				auxEvent->recompensas.push_back(hasRewards->second->AsString());
		}
	}
	else
		throw std::runtime_error("Evento sin recompensas / no recompensa mal especificado");

	generalData().getNPCData((npc::Personaje)personaje)->events.push_back(auxEvent);
}

/* Para los eventos debemos especificar sus condiciones. Eso lo haremos añadiendo
* condiciones en la seccion condiciones del json.
* Luego, opcionalmente se puede especificar las caracteristicas de los paquetes.
* Si no se ponen caracteristicas, saldran random del dia correspondiente.
* En caso de necesitar algo que no se pueda hacer por el json, he hecho la funcion
* special, que si pones special = id, accede a esa seccion del codigo para que lo hagas
* como necesites. */
void NPCeventSystem::readNPCEventData() {

	std::unique_ptr<JSONValue> jsonFile(JSON::ParseFromFile("recursos/data/npcData.json"));

	if (jsonFile == nullptr || !jsonFile->IsObject()) {
		throw "Something went wrong while load/parsing npcData";
	}

	JSONObject root = jsonFile->AsObject();
	JSONValue* jValueRoot = nullptr;

	for (int i = 0; i < 7; i++)
	{
		std::string aux = generalData().personajeToString((npc::Personaje)i);
		jValueRoot = root[aux];

		JSONObject jObject = jValueRoot->AsObject();
		JSONObject eventObject = jObject["Eventos"]->AsObject();

		if (i < 2) // npc grandes
		{
			for (int j = 0; j < 14; j++)
			{
				readNPCevent(eventObject, i, j);
			}
		}
		else
		{
			for (int j = 0; j < 5; j++)
			{
				readNPCevent(eventObject, i, j);
			}
		}
		jValueRoot = nullptr;
	}
}
