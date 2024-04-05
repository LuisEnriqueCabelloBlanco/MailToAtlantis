#pragma once
#include "../architecture/Game.h"
#include "../architecture/Scene.h"
#include "../components/Transform.h"
#include "../components/Paquete.h"
#include <components/Herramientas.h>
#include "../sistemas/PaqueteBuilder.h"
#include "../components/DialogManager.h"

namespace ecs {
	class TutorialScene : public Scene
	{
	public:
		TutorialScene();
		virtual ~TutorialScene();

		virtual void update() override;
		virtual void render() override;
		void close() override;
		void init() override;

		void closeConversation();
	private:
		void createManual();
		void createMiniManual();
		void createSpaceManual();

		void createClock();
		void createInks();
		void createOneInk(TipoHerramienta type);
		void createCharacter();

		Entity* manualEnt_;
		Entity* miniManualEnt_;

		PaqueteBuilder* mPaqBuild_;

		// entidades de dialogo
		Entity* boxBackground;
		Entity* textDialogue;
		void createDialogueBox();
		DialogManager dialogMngr_;

		Entity* sistemaFuturo;

		void activateDialogue();
	};
}