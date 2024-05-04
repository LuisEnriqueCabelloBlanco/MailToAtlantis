#pragma once
#include <utils/checkML.h>
#include "../architecture/Scene.h"
#include "../components/Transform.h"
#include <components/Herramientas.h>
#include "../sistemas/PaqueteBuilder.h"
#include "../sistemas/PipeManager.h"
#include "components/DialogManager.h"
#include "sistemas/WorkRestrictionsSystem.h"
#include "../sistemas/SpecialObjectsFactory.h"

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

        void nextIteration(int it);

        void updateIntroDialogue();

        void delayedCallback(float time, SimpleCallback call);

        ecs::Entity* createGarbage();

        void createIntroPackage();

        int introIteration;

        bool waitingCallback;
        SimpleCallback call_;
        Uint32 timer_;
        Uint32 timeToCall_;

        PaqueteBuilder* mPaqBuild_;

        PipeManager* mPipeMngr_;

        WorkRestrictionsSystem mWorkRes;

        SpecialObjectsFactory* specialFactory_;

        DialogManager mDialogManager;

        ecs::Entity* tubo_;
    };
}