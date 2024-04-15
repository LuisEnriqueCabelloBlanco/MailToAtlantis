#include "NPCeventSystem.h"
#include "../json/JSON.h"
#include <random>
#include <algorithm>
#include "../architecture/GeneralData.h"
#include "../sistemas/PaqueteBuilder.h"
#include "../architecture/Game.h"

NPCeventSystem::NPCeventSystem() {

}

NPCeventSystem::~NPCeventSystem() {

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

}

void NPCeventSystem::addActiveEvent(NPCevent* event) {

}

void NPCeventSystem::minigameOver() {
	while (areTherePaquetesNPC())
		paquetesNPCs.pop_back();
}

void NPCeventSystem::readNPCEventData() {

	std::unique_ptr<JSONValue> jsonFile(JSON::ParseFromFile("recursos/data/npcData.json"));

	if (jsonFile == nullptr || !jsonFile->IsObject()) {
		throw "Something went wrong while load/parsing npcData";
	}

	JSONObject root = jsonFile->AsObject();
	JSONValue* jValueRoot = nullptr;

	for (int i = 0; i < 7; i++)
	{
		std::string aux = generalData().personajeToString((GeneralData::Personaje)i);
		jValueRoot = root[aux];

		JSONObject jObject = jValueRoot->AsObject();

		if (i < 2) // npc grandes
		{
			JSONObject eventObject = jObject["Eventos"]->AsObject();

			/* Para los eventos debemos especificar sus condiciones. Eso lo haremos añadiendo
			* condiciones en la seccion condiciones del json.
			* Luego, opcionalmente se puede especificar las caracteristicas de los paquetes.
			* Si no se ponen caracteristicas, saldran random del dia correspondiente.
			* En caso de necesitar algo que no se pueda hacer por el json, he hecho la funcion
			* special, que si pones special = id, accede a esa seccion del codigo para que lo hagas
			* como necesites. */
			for (int j = 0; j < 14; j++)
			{
				NPCevent* auxEvent = new NPCevent();

				JSONObject currentEvent = eventObject.find(std::to_string(j + 1))->second->AsObject();
				auxEvent->numPaquetes = currentEvent.find("numPaquetes")->second->AsNumber();
				auxEvent->numPaquetesToComplete = currentEvent.find("numPaquetesParaCompletar")->second->AsNumber();

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
			#pragma region Paquetes
					auto paquetesArray = currentEvent.find("paquetes");
					if (paquetesArray != currentEvent.end())
					{
						JSONObject obj = paquetesArray->second->AsObject();

						// SI ESPECIFICAMOS LOS DATOS PARA CADA PAQUETE 
						if (obj.find("1") != obj.end())
						{
							for (auto paq : obj) {
								PaqueteBuilder paqBuild(gm().getScene(ecs::sc::MAIN_SCENE));

								bool legal = true;

								std::string rem = paqBuild.remitenteRND();
								auto hasRemitente = obj.find("remitente");
								if (hasRemitente != obj.end())
									rem = hasRemitente->second->AsString();

								Distrito dist = (Distrito)sdlutils().rand().nextInt(0, getTubesAmount());
								auto hasDistrito = obj.find("distrito");
								if (hasDistrito != obj.end())
								{

									dist = (Distrito)fromStringToDistrito(
										hasDistrito->second->AsString());

									if (legal && dist == Erroneo)
										legal = false;
								}

								Calle calle = (Calle)sdlutils().rand().nextInt(0, 3);
								auto hasCalle = obj.find("calle");
								if (hasCalle != obj.end())
								{
									calle = stringToCalle(hasCalle->second->AsString());

									if (legal && calle == Erronea)
										legal = false;
								}

								TipoPaquete tipo = (TipoPaquete)sdlutils().rand().nextInt(0, 5);
								auto hasTipo = obj.find("tipoPaquete");
								if (hasTipo != obj.end())
								{
									tipo = stringToTipoPaquete(hasTipo->second->AsString());
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
											nivelPeso = stringToNivelPeso(hasNivelPeso->second->AsString());
										}

										auto hasPesoKG = obj.find("pesoKG");
										if (hasPesoKG != obj.end())
											peso = hasPesoKG->second->AsNumber();

									}
								}

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
						else // SI LOS DATOS SON PARA TODOS LOS PAQUETES
						{
							for (int i = 0; i < auxEvent->numPaquetes; i++)
							{
								PaqueteBuilder paqBuild(gm().getScene(ecs::sc::MAIN_SCENE));

								bool legal = true;

								std::string rem = paqBuild.remitenteRND();
								auto hasRemitente = obj.find("remitente");
								if (hasRemitente != obj.end())
									rem = hasRemitente->second->AsString();

								Distrito dist = (Distrito)sdlutils().rand().nextInt(0, getTubesAmount());
								auto hasDistrito = obj.find("distrito");
								if (hasDistrito != obj.end())
								{

									dist = (Distrito)fromStringToDistrito(
										hasDistrito->second->AsString());

									if (legal && dist == Erroneo)
										legal = false;
								}

								Calle calle = (Calle)sdlutils().rand().nextInt(0, 3);
								auto hasCalle = obj.find("calle");
								if (hasCalle != obj.end())
								{
									calle = stringToCalle(hasCalle->second->AsString());

									if (legal && calle == Erronea)
										legal = false;
								}

								TipoPaquete tipo = (TipoPaquete)sdlutils().rand().nextInt(0, 5);
								auto hasTipo = obj.find("tipoPaquete");
								if (hasTipo != obj.end())
								{
									tipo = stringToTipoPaquete(hasTipo->second->AsString());
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
											nivelPeso = stringToNivelPeso(hasNivelPeso->second->AsString());
										}

										auto hasPesoKG = obj.find("pesoKG");
										if (hasPesoKG != obj.end())
											peso = hasPesoKG->second->AsNumber();

									}
								}

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
					}
					else
						throw std::runtime_error("Fallo cargando paquetes de evento");
#pragma endregion

#pragma region Condiciones
					auto condicionesArray = currentEvent.find("condiciones");
					if (condicionesArray != currentEvent.end())
					{
						JSONObject obj = condicionesArray->second->AsObject();

						// SI ESPECIFICAMOS LAS CONDICIONES PARA CADA PAQUETE 
						if (obj.find("1") != obj.end())
						{
							for (auto paq : obj)
							{
								JSONObject pqConditions = paq.second->AsObject();

								auto hasRemitente = pqConditions.find("remitente");
								if (hasRemitente != pqConditions.end()) {
									std::string aux = hasRemitente->second->AsString();
									pqEvento->addCondition([aux](Paquete* p) -> bool {
										return p->getRemitente() == aux;
										});
								}

								auto hasDistrito = pqConditions.find("distrito");
								if (hasDistrito != pqConditions.end()) {

									Distrito aux = (Distrito)fromStringToDistrito(hasDistrito->second->AsString());
									pqEvento->addCondition([aux](Paquete* p) -> bool {
										return p->getDistrito() == aux;
										});
								}

								auto hasCalle = pqConditions.find("calleMarcada");
								if (hasCalle != pqConditions.end()) {
									Calle aux = (Calle)stringToCalle(hasCalle->second->AsString());
									pqEvento->addCondition([aux](Paquete* p) -> bool {
										return p->getCalleMarcada() == aux;
										});
								}

								auto hasTipo = pqConditions.find("tipoPaquete");
								if (hasTipo != pqConditions.end()) {
									TipoPaquete aux = stringToTipoPaquete(hasTipo->second->AsString());
									pqEvento->addCondition([aux](Paquete* p) -> bool {
										return p->getTipo() == aux;
										});
								}

								auto hasPeso = pqConditions.find("peso");
								if (hasPeso != pqConditions.end()) {
									NivelPeso aux = stringToNivelPeso(hasPeso->second->AsString());
									pqEvento->addCondition([aux](Paquete* p) -> bool {
										return p->getPeso() == aux;
										});
								}

								auto hasPesoKG = pqConditions.find("pesoKG");
								if (hasPesoKG != pqConditions.end()) {
									int aux = hasPesoKG->second->AsNumber();
									pqEvento->addCondition([aux](Paquete* p) -> bool {
										return p->getCantidadPeso() == aux;
										});
								}

								auto hasFragil = pqConditions.find("fragil");
								if (hasFragil != pqConditions.end()) {
									bool aux = hasFragil->second->AsBool();
									pqEvento->addCondition([aux](Paquete* p) -> bool {
										return aux && p->getFragil() && p->getEnvuelto();
										});
								}

								auxEvent->compPaqueteEvento.push_back(pqEvento);
							}
						}
						else // SI LAS CONDICIONES SON PARA TODOS LOS PAQUETES
						{
							for (int i = 0; i < auxEvent->numPaquetes; i++)
							{
								PaqueteEvento* pqEvento = new PaqueteEvento(auxEvent);


								auxEvent->compPaqueteEvento.push_back(pqEvento);
							}
						}
					}
					else
						throw std::runtime_error("Fallo cargando condiciones de evento");

#pragma endregion
				}
#pragma region Recompensas

#pragma endregion




				npcData[i]->events.push_back(auxEvent);
			}
		}
		else
		{
			std::vector<bool> diasDanEventos;
			jObject = jValueRoot->AsObject();
			JSONObject jDiasEvento = jObject.find("DiasConEvento")->second->AsObject();
			// leemos los 14 booleanos
			for (int i = 0; i < 14; i++)
			{
				diasDanEventos.push_back(jDiasEvento.find(std::to_string(i + 1))->second->AsBool());
			}
			npcData.push_back(new NPCMenorData(stringToFelicidad(felicidadStr), diasDanEventos));

			// leemos los eventos
			JSONObject eventObject = jObject["Eventos"]->AsObject();

			/* Para los eventos debemos especificar sus condiciones. Eso lo haremos añadiendo
			* condiciones en la seccion condiciones del json.
			* Luego, opcionalmente se puede especificar las caracteristicas de los paquetes.
			* Si no se ponen caracteristicas, saldran random del dia correspondiente.
			* En caso de necesitar algo que no se pueda hacer por el json, he hecho la funcion
			* special, que si pones special = id, accede a esa seccion del codigo para que lo hagas
			* como necesites.
			*/
			for (int j = 0; j < 5; j++)
			{
				NPCevent* auxEvent = new NPCevent();

				JSONObject currentEvent = eventObject.find(std::to_string(j + 1))->second->AsObject();
				auxEvent->numPaquetes = currentEvent.find("numPaquetes")->second->AsNumber();
				auxEvent->numPaquetesToComplete = currentEvent.find("numPaquetesParaCompletar")->second->AsNumber();

#pragma region Tipo 1
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
#pragma endregion

#pragma region Tipo 2
				auto isPremadePaquete = currentEvent.find("paquetes");
				if (isPremadePaquete != currentEvent.end())
				{
					JSONObject obj = isPremadePaquete->second->AsObject();

					for (auto paq : obj) {
						JSONObject paqueteConditions = paq.second->AsObject();
						PaqueteBuilder paqBuild(gm().getScene(ecs::sc::MAIN_SCENE));

						bool legal = true;

						std::string rem = paqBuild.remitenteRND();
						auto hasRemitente = paqueteConditions.find("remitente");
						if (hasRemitente != paqueteConditions.end())
							rem = hasRemitente->second->AsString();

						Distrito dist = (Distrito)sdlutils().rand().nextInt(0, getTubesAmount());
						auto hasDistrito = paqueteConditions.find("distrito");
						if (hasDistrito != paqueteConditions.end())
						{
							std::string aux = hasDistrito->second->AsString();
							if (aux == "Hestia")
								dist == Hestia;
							else if (aux == "Artemisa")
								dist == Artemisa;
							else if (aux == "Demeter")
								dist == Demeter;
							else if (aux == "Hefesto")
								dist == Hefesto;
							else if (aux == "Hermes")
								dist == Hermes;
							else if (aux == "Apolo")
								dist == Apolo;
							else if (aux == "Poseidon")
								dist == Poseidon;
							else if (aux == "Erroneo")
							{
								dist == Erroneo;
								legal = false;
							}

						}

						Calle calle = (Calle)sdlutils().rand().nextInt(0, 3);
						auto hasCalle = paqueteConditions.find("calle");
						if (hasCalle != paqueteConditions.end())
						{
							std::string aux = hasCalle->second->AsString();
							if (aux == "C1")
								calle = C1;
							else if (aux == "C2")
								calle = C2;
							else if (aux == "C3")
								calle = C3;
							else if (aux == "Erroneo")
							{
								calle = Erronea;
								legal = false;
							}
						}

						TipoPaquete tipo = (TipoPaquete)sdlutils().rand().nextInt(0, 5);
						auto hasTipo = paqueteConditions.find("tipoPaquete");
						if (hasTipo != paqueteConditions.end())
						{
							std::string aux = hasTipo->second->AsString();
							if (aux == "Alimento")
								tipo = Alimento;
							else if (aux == "Medicinas")
								tipo == Medicinas;
							else if (aux == "Joyas")
								tipo == Joyas;
							else if (aux == "Materiales")
								tipo == Materiales;
							else if (aux == "Armamento")
								tipo == Armamento;
						}

						int peso = 0;
						NivelPeso nivelPeso = Ninguno;
						bool fragil = false;

						if (generalData().getPaqueteLevel() > 1)
						{
							fragil = sdlutils().rand().nextInt(0, 10);
							fragil = fragil < 2;
							auto hasFragil = paqueteConditions.find("fragil");
							if (hasFragil != paqueteConditions.end())
								fragil = hasFragil->second->AsBool();

							if (generalData().getPaqueteLevel() > 2)
							{
								// esto toma los valores brutamente del json
								nivelPeso = paqBuild.pesoRND(20, 30, peso);
								auto hasNivelPeso = paqueteConditions.find("peso");
								if (hasNivelPeso != paqueteConditions.end())
								{
									std::string aux = hasNivelPeso->second->AsString();
									if (aux == "Bajo")
										nivelPeso == Bajo;
									else if (aux == "Medio")
										nivelPeso == Medio;
									else if (aux == "Alto")
										nivelPeso == Alto;
									else if (aux == "Ninguno")
										nivelPeso == Ninguno;
								}

								auto hasPesoKG = paqueteConditions.find("pesoKG");
								if (hasPesoKG != paqueteConditions.end())
									peso = hasPesoKG->second->AsNumber();

							}
						}

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
#pragma endregion

#pragma region Tipo 3
				auto isConditionEvent = currentEvent.find("condiciones");
				if (isConditionEvent != currentEvent.end()) {

					JSONObject paqueteConditions = isConditionEvent->second->AsObject();
					PaqueteBuilder paqBuild(gm().getScene(ecs::sc::MAIN_SCENE));

					bool legal = true;

					std::string rem = paqBuild.remitenteRND();
					auto hasRemitente = paqueteConditions.find("remitente");
					if (hasRemitente != paqueteConditions.end())
						rem = hasRemitente->second->AsString();

					Distrito dist = (Distrito)sdlutils().rand().nextInt(0, getTubesAmount());
					auto hasDistrito = paqueteConditions.find("distrito");
					if (hasDistrito != paqueteConditions.end())
					{
						std::string aux = hasDistrito->second->AsString();
						if (aux == "Hestia")
							dist == Hestia;
						else if (aux == "Artemisa")
							dist == Artemisa;
						else if (aux == "Demeter")
							dist == Demeter;
						else if (aux == "Hefesto")
							dist == Hefesto;
						else if (aux == "Hermes")
							dist == Hermes;
						else if (aux == "Apolo")
							dist == Apolo;
						else if (aux == "Poseidon")
							dist == Poseidon;
						else if (aux == "Erroneo")
						{
							dist == Erroneo;
							legal = false;
						}

					}

					Calle calle = (Calle)sdlutils().rand().nextInt(0, 3);
					auto hasCalle = paqueteConditions.find("calle");
					if (hasCalle != paqueteConditions.end())
					{
						std::string aux = hasCalle->second->AsString();
						if (aux == "C1")
							calle = C1;
						else if (aux == "C2")
							calle = C2;
						else if (aux == "C3")
							calle = C3;
						else if (aux == "Erroneo")
						{
							calle = Erronea;
							legal = false;
						}
					}

					TipoPaquete tipo = (TipoPaquete)sdlutils().rand().nextInt(0, 5);
					auto hasTipo = paqueteConditions.find("tipoPaquete");
					if (hasTipo != paqueteConditions.end())
					{
						std::string aux = hasTipo->second->AsString();
						if (aux == "Alimento")
							tipo = Alimento;
						else if (aux == "Medicinas")
							tipo == Medicinas;
						else if (aux == "Joyas")
							tipo == Joyas;
						else if (aux == "Materiales")
							tipo == Materiales;
						else if (aux == "Armamento")
							tipo == Armamento;
					}

					int peso = 0;
					NivelPeso nivelPeso = Ninguno;
					bool fragil = false;

					if (generalData().getPaqueteLevel() > 1)
					{
						fragil = sdlutils().rand().nextInt(0, 10);
						fragil = fragil < 2;
						auto hasFragil = paqueteConditions.find("fragil");
						if (hasFragil != paqueteConditions.end())
							fragil = hasFragil->second->AsBool();

						if (generalData().getPaqueteLevel() > 2)
						{
							// esto toma los valores brutamente del json
							nivelPeso = paqBuild.pesoRND(20, 30, peso);
							auto hasNivelPeso = paqueteConditions.find("peso");
							if (hasNivelPeso != paqueteConditions.end())
							{
								std::string aux = hasNivelPeso->second->AsString();
								if (aux == "Bajo")
									nivelPeso == Bajo;
								else if (aux == "Medio")
									nivelPeso == Medio;
								else if (aux == "Alto")
									nivelPeso == Alto;
								else if (aux == "Ninguno")
									nivelPeso == Ninguno;
							}

							auto hasPesoKG = paqueteConditions.find("pesoKG");
							if (hasPesoKG != paqueteConditions.end())
								peso = hasPesoKG->second->AsNumber();

						}
					}

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
#pragma endregion

				npcData[i]->events.push_back(auxEvent);
			}
		}
		jValueRoot = nullptr;
	}
}