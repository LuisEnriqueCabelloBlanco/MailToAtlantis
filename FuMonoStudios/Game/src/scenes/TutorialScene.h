#pragma once
#include "../architecture/Game.h"
#include "../architecture/Scene.h"
#include "../components/Transform.h"
#include "../components/Paquete.h"
#include <components/Herramientas.h>
#include "../sistemas/PaqueteBuilder.h"
#include "../components/DialogManager.h"
#include "../sistemas/TutorialSystem.h"

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

		void packageSent();

		enum PackageTutorial { Primero, Segundo, Random};
		void createPackage(PackageTutorial pt);
	private:
		void createManual();
		void createMiniManual();
		void createSpaceManual();

		void createStamp(TipoHerramienta type);
		void createClock();
		void createInks();
		void createOneInk(TipoHerramienta type);
		void createTubo(pq::Distrito dist, bool);

		Entity* manualEnt_;
		Entity* miniManualEnt_;

		TutorialSystem* tutorialSys_;

		PaqueteBuilder* mPaqBuild_;

		// lo mismo que el drag and drop pero funciona unicamente 
		// si el canDrag esta activo
		class DragAndDropTutorial : public DragAndDrop
		{
		public:
			DragAndDropTutorial(bool usingNearest, TutorialSystem* tutSys_) 
				: DragAndDrop(usingNearest) {
				tutorialSys_ = tutSys_;
			}
			DragAndDropTutorial(bool usingNearest, bool usingOwnCallback, TutorialSystem* tutSys_)
				: DragAndDrop(usingNearest, usingOwnCallback) {
				tutorialSys_ = tutSys_;
			}
			void update() {
				if (tutorialSys_->canDrag)
					DragAndDrop::update();

			}
		private:
			TutorialSystem* tutorialSys_;
		};
	};
}