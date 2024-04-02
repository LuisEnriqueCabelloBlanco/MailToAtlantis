#pragma once
#include "../architecture/Scene.h"
#include "../architecture/ecs.h"

class Game;

namespace ecs {
    class PauseScene :
        public Scene
    {
    public:
        PauseScene();
        virtual ~PauseScene();

        void init() override;
        void leaveMenu();
    };
}
