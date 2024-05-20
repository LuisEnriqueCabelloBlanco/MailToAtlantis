#pragma once
#include <vector>

class Entity;

namespace ecs {

	using cmpId_t = uint8_t;
	//Namespace que contiene los IDs de todos los componentes
	namespace cmp {
		enum cmpId : cmpId_t {
			TRANSFORM = 0,
			IMAGE,
			HOVERSENSOR,
			HOVERLAYER,
			CTRL,
			CLICKEABLE,
			DRAGANDDROP,
			TRIGGER,
			PAQUETE,
			CHECKER,
			HERRAMIENTAS,
			MULTIPLETEXTURES,
			GRAVITY,
			DIALOGUE,
			MOVERTRANSFORM,
			WRAP,
			SELFDESTRUCT,
			DEPTH,
			ERROR_NOTE,
			NPC_EXCLAMATION,
			IMAGEWITHLIGHT,
			DELAYEDCALLBACK,
			CLOCKAUX,
			BALANZA,
			ROTARTRANSFORM,
			CRISTALBALL,
			BOMBAUX,
			POLVOSAUX,
			// do not remove this
			_LAST_CMP_ID
		};
		constexpr cmpId_t maxComponentId = _LAST_CMP_ID;
	}

	template<typename T>
	constexpr cmpId_t cmpId = T::id;


	using scId_t = uint8_t;
	//Namespace para etiquetar las escenas
	namespace sc {
		enum sceneId : scId_t {
			MAIN_SCENE,
			EXPLORE_SCENE,
			END_WORK_SCENE,
			MENU_SCENE,
			PAUSE_SCENE,
			END_SCENE,
			TUTORIAL_SCENE,
			CONFIG_SCENE,
			INTRO_SCENE,
			DEATH_SCENE,
			FINALS_INFO_SCENE,
			_LAST_SCENE,
			NULL_SCENE
		};
		constexpr sceneId maxSceneId = _LAST_SCENE;
	}
#define __SC_DECL__(cId) constexpr static ecs::scId_t id = cId;

	using scUpg_t = uint8_t;
	//Namespace para etiquetar las mejoras
	namespace upg {
		enum upgradeId : scUpg_t {
			MONEY_UPGRADE,
			SELLO_UPGRADE,
			ENVOLVER_UPGRADE,
			BOLA_UPGRADE,
			FALLOS_UPGRADE,
			BALANZA_UPGRADE,
			MANUAL_UPGRADE,
			_LAST_UPGRADE
		};
		constexpr upgradeId maxUpgradeId = _LAST_UPGRADE;
	}

	using lyId_t = uint8_t;
	//Namespace de las capas
	//El orden de las capas es de atr�s hacia adelante;
	namespace layer {
		enum layerId : lyId_t
		{
			INACTIVE,
			BACKGROUND,
			LIGHT,
			DEFAULT,
			BIN,
			MANUALSPACE,
			MINIMANUAL,
			BALANZABASE,
			BALANZA,
			NUMBERS,
			PACKAGE,
			INFO_PACKAGE,
			INK,
			STAMP,
			BALANZAB,
			OFFICEELEMENTS,
			RED_LINES,
			WRAP_POINTS,
			TAPE,
			BLUEPOINT,
			MANUAL,
			FOREGROUND,
			UI,
			
			_LAST_LAYER
		};
		constexpr layerId maxLayerId = _LAST_LAYER;
	}
	// a macro for component identifier declaration, e.g., __CMPID_DECL__(ecs::_TRANSFORM)
	// expands to:
	//
	//   constexpr static ecs::cmpId_type id = ecs::_TRANSFORM;
	//
	#define __CMP_DECL__(cId) constexpr static ecs::cmpId_t id = cId;
}

