#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <scenes/ConfigScene.h>

class Game;

namespace ecs {
    class PauseScene : public ConfigScene
    {
        __SC_DECL__(ecs::sc::PAUSE_SCENE)
    public:
        PauseScene();
        virtual ~PauseScene();

        void init() override;
        void close() override;
    };
}
