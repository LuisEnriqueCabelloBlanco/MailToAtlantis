#include "FinalsInfoScene.h"
#include <sistemas/ComonObjectsFactory.h>

FinalsInfoScene::FinalsInfoScene()
{



}

void FinalsInfoScene::init()
{
    // Fondo escena
    factory_->setLayer(ecs::layer::BACKGROUND);
    ecs::Entity* fondo = factory_->createImage(Vector2D(0, 0), &sdlutils().images().at("fondoAzul"));

    // IDs de las imagenes de los emblemasNpcs
    std::vector<std::string> npcIDs_ = { "SoldadoEmblema", "PaulinoEmblema", "PancracioEmblema",
    "GloverEmblema", "GargafielEmblema", "AnemosEmblema", "abiEmblema" };
    factory_->setLayer(ecs::layer::UI);
    // Creamos los npcButtons_
    int i = 0;
    for (const auto& id : npcIDs_) {
        CallbackClickeable callback = [this]() {
            std::cout << "mostrar";
        };
        Texture* emblemaTex = &sdlutils().images().at(id);
        ecs::Entity* button = factory_->createImageButton(Vector2D(250 * i, 0), Vector2D(250, 250), emblemaTex, callback, "click");
        i++;

        factory_->addHoverColorMod(button, build_sdlcolor(0xaaaaaaff));
    }
   


    setActiveButtons(true, npcButtons_); // activa los npcButtons_
}

void FinalsInfoScene::update()
{
    //hola si hola, hacia falta llamar al update para que funcionaran las cosas, al hacer override no se estaba llamano al metodo padre
    Scene::update();
}

void FinalsInfoScene::close()
{

}

void FinalsInfoScene::setActiveButtons(bool enable, std::vector<ecs::Entity*> buttons)
{
    
}

