#pragma once
#include <utils/checkML.h>
#include <architecture/Scene.h>
#include <components/Transform.h>
#include <components/Herramientas.h>
#include <sistemas/PaqueteBuilder.h>
#include <components/DialogManager.h>

namespace ecs
{
    class IntroScene : public ecs::Scene
    {
        __SC_DECL__(ecs::sc::INTRO_SCENE)
    public:
        IntroScene();
        ~IntroScene();

        void update() override;

        void init() override;
    protected:

        void updateIteration(int it);

        void updateIntroDialogue();

        void delayedCallback(float time, SimpleCallback call);

        void cambiarFondo(const std::string& background);

        ecs::Entity* createGarbage();

        void createIntroPackage();

        ecs::Entity* createBottle();

        void nextIteration();

        int introIteration;

        bool waitingCallback;
        SimpleCallback call_;
        Uint32 timer_;
        Uint32 timeToCall_;

        PaqueteBuilder* mPaqBuild_;

        DialogManager mDialogManager;

        ecs::Entity* fondo_;
        ecs::Entity* tubo_;
        ecs::Entity* bottle_;
        ecs::Entity* carta_;
        ecs::Entity* jefe_;
    };
}