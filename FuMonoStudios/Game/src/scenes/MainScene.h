#pragma once
#include <utils/checkML.h>
#include "../architecture/Game.h"
#include "../architecture/Scene.h"
#include "../components/Transform.h"
#include "../components/Paquete.h"
#include <components/Herramientas.h>
#include "../sistemas/PaqueteBuilder.h"
#include "components/DialogManager.h"

namespace ecs {
    class Game;
    class MainScene :public Scene
    {
    public:
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
    private:
        void createManual(int NumPages);
        void createMiniManual();
        void createSpaceManual();
        void createMultipleStamp();

        //void createTubo(Paquete::Distrito dist, bool desbloqueado);

        void createClock();


        //void createSelladores();
        void createGarbage();
        void createCinta();
        void createBalanza();
        void createTubo(pq::Distrito dist, bool);
        void createStamp(TipoHerramienta type);
        
        void createInks();
        void createOneInk(TipoHerramienta type);

        void updateToolsPerDay(int dia);

        int fails_;
        int correct_;

        float timer_;
        bool timerPaused_;

        DialogManager dialogMngr_;
        
        ecs::Entity* createCharacter(Vector2D pos, const std::string& character, float scale);

        void startWork();

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

        //El pinche paquete builder para no crear uno en cada paquete
        PaqueteBuilder* mPaqBuild_;
    };
}

