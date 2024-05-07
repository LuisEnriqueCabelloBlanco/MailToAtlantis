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
        Texture* audioValueTexture_;
        Entity* audioValueEnt_;

        //Texture* screenModeTexture_;
        //Entity* screenModeEnt_;
    public:
        ConfigScene();
        virtual ~ConfigScene();

        void init() override;
        void updateValue(Texture* texture, Entity* entity, int index);
        void changeToMenuScene();
    };
}

