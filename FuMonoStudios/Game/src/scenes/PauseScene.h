#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
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
    };
}
