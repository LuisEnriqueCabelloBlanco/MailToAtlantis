#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <architecture/Scene.h>

class Game;

namespace ecs {
    class MainMenu :
        public Scene
    {
        __SC_DECL__(ecs::sc::MAIN_SCENE)
    public:
        MainMenu();
        virtual ~MainMenu();

        void init() override;
    };
}

