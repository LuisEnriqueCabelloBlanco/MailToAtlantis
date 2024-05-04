#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "../architecture/Game.h"
#include "../architecture/Scene.h"
#include "../components/Transform.h"
#include "../components/Paquete.h"
#include "../entities/CristalBall.h"
#include <components/Herramientas.h>
#include "../sistemas/PaqueteBuilder.h"
#include "../sistemas/PipeManager.h"
#include "components/DialogManager.h"
#include "sistemas/WorkRestrictionsSystem.h"
#include "../sistemas/SpecialObjectsFactory.h"

namespace ecs {
    class Game;
    class MainScene :public Scene
    {
    public:
        __SC_DECL__(ecs::sc::MAIN_SCENE)
        MainScene();
        virtual ~MainScene();

        virtual void update() override;
        virtual void render() override;
        void close() override;
        void init() override;
        void switchTimer() { timerPaused_ = !timerPaused_; }
        void setTimer(float time) { timer_ = time; }
        void createPaquete(int lv);
        void createErrorMessage(Paquete* paqComp, bool, bool);
        void createExclamationPoint();

    protected:
        virtual std::unordered_map<std::string, ecs::Entity*> createManual(int NumPages);
        virtual ecs::Entity* createMiniManual();
        ecs::Entity* createSpaceManual();
        void createMultipleStamp();
        void createBalanzaDigital();

        //void createTubo(Paquete::Distrito dist, bool desbloqueado);

        ecs::Entity* createClock();
        ecs::Entity* createBolaCristal();


        //void createSelladores();
        virtual ecs::Entity* createGarbage();
        ecs::Entity* createCinta();
        virtual std::unordered_map<std::string, ecs::Entity*> createBalanza();
        ecs::Entity* createTubo(pq::Distrito dist, bool);
        ecs::Entity* createStamp(TipoHerramienta type);
        
        void createInks();
        ecs::Entity* createOneInk(TipoHerramienta type);

        void updateToolsPerDay(int dia);

        int fails_;
        int correct_;

        float timer_;
        bool timerPaused_;

        DialogManager dialogMngr_;
        
        
        ecs::Entity* createCharacter(Vector2D pos, const std::string& character, float scale);

        void startWork();

        void newWorkEvent();

#ifdef DEV_TOOLS
        bool nextPacageCorrect_;
        bool stampsUnloked_;
        bool weightUnloked_;
        bool cintaUnloked_;
        int timeToAdd_;
        void makeDataWindow();
        void makeControlsWindow();
#endif // DEV_TOOLS

        Entity* timerEnt_;
        Texture* timerTexture_;

        Entity* manualEnt_;
        Entity* miniManualEnt_;

        std::vector<ecs::Entity*> tubos;

        CristalBall* bolaCrist_;

        //El pinche paquete builder para no crear uno en cada paquete
        PaqueteBuilder* mPaqBuild_;

        PipeManager* mPipeMngr_;

        WorkRestrictionsSystem mWorkRes;

        SpecialObjectsFactory* specialFactory_;
    };
}

