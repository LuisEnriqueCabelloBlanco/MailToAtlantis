#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <vector>
#include <string>
#include <unordered_map>
#include <json/JSON.h>
//#include <sistemas/NPCeventSystem.h>
class NPCevent;
namespace npc {
	// enum con tipos de felicidad
	enum Felicidad { SeFue, Minima, Mala, Normal, Buena, Maxima, NoHabladoAun };

	// enum con el nombre de todos los NPC
	enum Personaje {
		Vagabundo, Secretario, Campesino, Artesano, Tarotisa, Soldado, Contable, Jefe
	};


	const std::unordered_map<Personaje, std::string> personajeToString = {
		{Vagabundo,"Vagabundo"},
		{Secretario,"Secretario"},
		{Campesino,"Campesino"},
		{Artesano,"Artesano"},
		{Tarotisa, "Tarotisa"},
		{Soldado, "Soldado"},
		{Contable, "Contable"},
		{Jefe, "Jefe"}
	};

	const std::unordered_map<Felicidad, std::string> happinessToString = {
		{Minima, "M�nima"},
		{Mala, "Mala"},
		{Normal, "Normal"},
		{Buena, "Buena"},
		{Maxima, "Maxima"},
		{NoHabladoAun, "No Hablado Aun"}
	};

	// Los datos de los NPC deben actualizarse al acabar cada d�a.
	// Recogen datos sobre su felicidad, as� como que dialogo deben ense�ar.
	// Al iniciarse, su felicidad estar� en NoHabladoAun, y al sacar su
	// primer di�logo cambiar� a Normal.
	// NPC MENORES: El bool giveEvent dicta si debe dar evento (true) o dar
	// un dialogo generico (false). El int iteration itera sobre los 3 posibles
	// dialogos genericos que tiene el personaje.
	// NPC GRANDES: El bool postConversation si es true, significa que ya se 
	// ha hablado con el una vez, y sacara el dialogo mas corto que sale despues
	// del dialogo original de ese dia.
	// 
	// Al acabar el d�a se debe llamar a setupDayData() para reiniciar las 
	// variables y ajustar datos segun el dia
	// 
	// NOTA IMPORTANTE: POSBILEMENTE SE PONDRA AQUI EL TEMA DE LAS CONDICIONES
	// Y LOS EVENTOS DE CADA NPC, AUN NO ESTA IMPLEMENTADO, SOLO ESTA PUESTO
	// LO DE LOS DIALOGOS
	struct NPCdata {
		/// <summary>
		/// Constructora de un NPC nuevo
		/// </summary>
		/// <param name="">Id del personaje</param>
		/// <param name="">Nuemro maximo de eventos</param>
		NPCdata(Personaje,int);
		virtual ~NPCdata();
		virtual std::pair<const std::string, int> getDialogueInfo() = 0;

		// esto solo lo usa el NPCmenor
		virtual void iterateDialogues() = 0;
		virtual void setupDayData() = 0;
		virtual NPCevent* getEvent() = 0;
		/// <summary>
		/// Actualiza los datos del npc a los indicados en el fichero de guardado
		/// </summary>
		/// <param name="obj"></param>
		virtual void loadDataFromSaveFile(JSONObject& obj);

		Personaje npcId;
		Felicidad felicidad;
		int numFelicidad;
		int numMisionesAceptadas;
		bool postConversation;
		bool misionAceptada;
		// el primero es si ha sido completado, el segundo es si ha sido succesful
		std::vector<std::pair<bool, int>> eventosCompletados;
		std::vector<NPCevent*> events;
		std::string introText;
	};

#pragma region NPCdata



	struct NPCMenorData : public NPCdata {
		virtual ~NPCMenorData();
		NPCMenorData(Felicidad Felicidad, std::vector<bool>& DiasDanEvento);
		/// <summary>
		/// Constructora que lee los datos del jsonObject de npcData.json
		/// </summary>
		/// <param name="charac"></param>
		/// <param name="obj"></param>
		NPCMenorData(Personaje charac, JSONObject& obj);

		std::pair<const std::string, int> getDialogueInfo() override;
		void iterateDialogues() override;
		void setupDayData() override;
		NPCevent* getEvent() override;
	private:
		void activateEvent();
		void deactivateEvent();

		std::vector<bool> diasDanEvento;

		bool giveEvent;
		int iteration;
		int lastIterationResult = -1;

	};

	struct NPCMayorData : public NPCdata {
		virtual ~NPCMayorData();
		NPCMayorData(Felicidad Felicidad);
		/// <summary>
		/// Constructora que lee los datos del jsonObject de npcData.json
		/// </summary>
		/// <param name="charId"></param>
		/// <param name="charRoot"></param>
		NPCMayorData(Personaje charId,JSONObject& charRoot);

		std::pair<const std::string, int> getDialogueInfo() override;
		void iterateDialogues() override {};
		void setupDayData() override;
		NPCevent* getEvent() override;

		int firstMision;
	};
}