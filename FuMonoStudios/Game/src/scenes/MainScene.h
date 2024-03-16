#pragma once
#include "../architecture/Scene.h"
#include "../components/Transform.h"
#include "../components/Paquete.h"

namespace ecs {
    class Game;
    class MainScene :public Scene
    {
    public:
        MainScene();
        virtual ~MainScene();

        virtual void update() override;
        void close() override;
        void init() override;
        void switchTimer() { timerPaused_ = !timerPaused_; }
        void setTimer(float time) { timer_ = time; }

        Font* getFont() { return timeFont_; }
    private:
        void createManual();
        void createClock();
        void createTubo(Paquete::Distrito dist);
        void createSelladores();
        void createPaquete(int lv);
        
        void updateTimer();

        int fails_;
        int correct_;
        float timer_;
        bool timerPaused_;
        Font* timeFont_;
        
        float x = 0;
        Vector2D clockCenter;
        Transform* trManecillaL;
        Transform* trManecillaS;
        // numeros magiquisimos para que roten las manecillas en el centro del reloj
        const Vector2D offsetL = Vector2D(-14, -10);
        const float radiusManL = 13.2;
        const float constRotL = 57.3;
        Vector2D offsetS = Vector2D(-14, -10);
        float radiusManS = 13.2;
        float constRotS = 57.3;


        // objects
        void initTexts(); // metodo auxiliar para limpio
        Entity* timerEnt_;
        Texture* timerTexture_;

        Entity* failsEnt_;
        Texture* failsTexture_ = nullptr;
        Entity* successEnt_;
        Texture* successTexture_ = nullptr;
#ifdef _DEBUG
        void updateFailsText();
#endif // _DEBUG
    };
}

