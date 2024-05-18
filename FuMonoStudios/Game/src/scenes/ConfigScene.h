#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <architecture/Scene.h>

class Game;

namespace ecs {
    class ConfigScene :
        public Scene
    {
    private:
        Texture* musicIconTexture_;
        Entity* musicIconEnt_;
        Texture* sfxIconTexture_;
        Entity* sfxIconEnt_;

        Texture* screenModeIconTexture_;
        Entity* screenModeIconEnt_;
        Texture* skipTutoIconTexture_;
        Entity* skipTutoIconEnt_;
    public:
        ConfigScene();
        virtual ~ConfigScene();

        void init() override;
        void changeToMenuScene();
    };
}

