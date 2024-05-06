#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <scenes/MainScene.h>
#include <sistemas/TutorialSystem.h>




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
		std::unordered_map<std::string, ecs::Entity*> createManual(int NumPages) override;
		std::unordered_map<std::string, ecs::Entity*> createBalanza() override;
		std::unordered_map<std::string, ecs::Entity*> createTubes() override;

		void deactivateTubos();
		void deactivateAllButOneTube(int tube);
		void activateTubos();
		void activateAllButOneTube(int tube);

		ecs::Entity* createGarbage() override; // empieza desactivada

		void activateGarbage();
		void deactivateGarbage();

		ecs::Entity* garbage_;

		TutorialSystem* tutorialSys_;

		bool balanzaUsed;
	};
}