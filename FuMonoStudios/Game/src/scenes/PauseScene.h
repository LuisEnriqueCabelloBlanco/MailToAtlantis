#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <scenes/ConfigScene.h>

class Game;

namespace ecs {
    class PauseScene : public ConfigScene
    {
    public:
        PauseScene();
        virtual ~PauseScene();

        void init() override;
    };
}
