#pragma once
#include <utils/checkML.h>
#include "../scenes/MainScene.h"
#include "../sistemas/TutorialSystem.h"




// Esta escena tiene los mismos objetos que la escena principal a excepcion
// del tutorialSystem, que va poniendo textos y permitiendo acciones en funcion
// del punto del tutorial en el que estamos
namespace ecs {
	class TutorialScene : public MainScene
	{
		friend TutorialSystem;
	public:
		TutorialScene();
		virtual ~TutorialScene();

		virtual void update() override;
		virtual void render() override;
		void close() override;
		void init() override;

		void packageSent();

		enum PackageTutorial { Primero, Segundo, Tercero, FallarAposta, Fragil, BalanzaTut, Random};
		ecs::Entity* createPackage(PackageTutorial pt);

		void createErrorMessage(Paquete* paqComp, bool basura, bool tuboIncorrecto);

		// getters
		Transform* getManualTransform() { return manualEnt_->getComponent<Transform>(); }
	private:

		ecs::Entity* createMiniManual() override;

		void deactivateTubos();
		void activateTubos();

		void createGarbage(); // empieza desactivada
		void activateGarbage();
		void deactivateGarbage();
		ecs::Entity* garbage_;

		TutorialSystem* tutorialSys_;

		bool balanzaUsed;

		// lo mismo que el drag and drop pero funciona unicamente 
		// si el canDrag esta activo
		class DragAndDropTutorial : public DragAndDrop
		{
		public:
			DragAndDropTutorial(bool usingNearest, TutorialSystem* tutSys_,const std::string& sound) 
				: DragAndDrop(usingNearest,sound) {
				tutorialSys_ = tutSys_;
			}
			DragAndDropTutorial(bool usingNearest, bool usingOwnCallback, TutorialSystem* tutSys_, const std::string& sound)
				: DragAndDrop(usingNearest, usingOwnCallback,sound) {
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