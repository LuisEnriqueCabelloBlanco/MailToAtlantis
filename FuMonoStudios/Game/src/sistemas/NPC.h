#pragma once
#include <vector>
#include <string>
#include <unordered_map>
namespace npc {
	// enum con tipos de felicidad
	enum Felicidad { Minima, Mala, Normal, Buena, Maxima, NoHabladoAun };

	// enum con el nombre de todos los NPC
	enum Personaje {
		Vagabundo, Secretario, Campesino, Artesano, Tarotisa, Soldado, Contable
	};


	const std::unordered_map<Personaje, std::string> personajeToString = {
		{Vagabundo,"Vagabundo"},
		{Secretario,"Secretario"},
		{Campesino,"Campesino"},
		{Artesano,"Artesano"},
		{Tarotisa, "Tarotisa"},
		{Soldado, "Soldado"},
		{Contable, "Contable"}
	};

	const std::unordered_map<Felicidad, std::string> happinessToString = {
		{Minima, "Mínima"},
		{Mala, "Mala"},
		{Normal, "Normal"},
		{Buena, "Buena"},
		{Maxima, "Maxima"},
		{NoHabladoAun, "No Hablado Aun"}
	};

	// Los datos de los NPC deben actualizarse al acabar cada día.
	// Recogen datos sobre su felicidad, así como que dialogo deben enseñar.
	// Al iniciarse, su felicidad estará en NoHabladoAun, y al sacar su
	// primer diálogo cambiará a Normal.
	// NPC MENORES: El bool giveEvent dicta si debe dar evento (true) o dar
	// un dialogo generico (false). El int iteration itera sobre los 3 posibles
	// dialogos genericos que tiene el personaje.
	// NPC GRANDES: El bool postConversation si es true, significa que ya se 
	// ha hablado con el una vez, y sacara el dialogo mas corto que sale despues
	// del dialogo original de ese dia.
	// 
	// Al acabar el día se debe llamar a setupDayData() para reiniciar las 
	// variables y ajustar datos segun el dia
	// 
	// NOTA IMPORTANTE: POSBILEMENTE SE PONDRA AQUI EL TEMA DE LAS CONDICIONES
	// Y LOS EVENTOS DE CADA NPC, AUN NO ESTA IMPLEMENTADO, SOLO ESTA PUESTO
	// LO DE LOS DIALOGOS
	struct NPCdata {

		Personaje npcId;
		Felicidad felicidad;
		virtual std::pair<const std::string, int> getDialogueInfo() = 0;

		// esto solo lo usa el NPCmenor
		virtual void iterateDialogues() = 0;
		virtual void setupDayData() = 0;
	};

#pragma region NPCdata



	struct NPCMenorData : public NPCdata {
		NPCMenorData(Felicidad Felicidad, std::vector<bool> DiasDanEvento);

		std::pair<const std::string, int> getDialogueInfo() override;
		void iterateDialogues() override;
		void setupDayData() override;
	private:
		void activateEvent();
		void deactivateEvent();

		std::vector<bool> diasDanEvento;

		bool giveEvent;
		int iteration;
	};

	struct NPCMayorData : public NPCdata {
		NPCMayorData(Felicidad Felicidad);

		std::pair<const std::string, int> getDialogueInfo() override;
		void iterateDialogues() override {};
		void setupDayData() override;
	private:
		bool postConversation;
	};
}