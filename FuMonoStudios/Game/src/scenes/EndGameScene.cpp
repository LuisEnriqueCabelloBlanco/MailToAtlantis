#include "EndGameScene.h"
#include "../sistemas/ComonObjectsFactory.h"
#include <vector>
#include <json/JSON.h>
#include <architecture/GeneralData.h>
#include <string>
#include <sdlutils/InputHandler.h>
#include <components/Render.h>

EndGameScene::EndGameScene()
{
    std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile("recursos/data/dialogos.json"));

    // check it was loaded correctly
    // the root must be a JSON object
    if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
        throw "Something went wrong while load/parsing dialogues";
    }
    // we know the root is JSONObject
    JSONObject root = jValueRoot->AsObject();

    


    for (int i = 0; i < 7; i++) {
        loadEnd((Personaje)i, root);
    }
    
    std::vector<Texture*> texturesArray = {
        &sdlutils().images().at("placeHolder"),
        &sdlutils().images().at("placeHolder"),
        &sdlutils().images().at("placeHolder"),
        &sdlutils().images().at("placeHolder"),
        &sdlutils().images().at("placeHolder"),
        &sdlutils().images().at("placeHolder"),
        &sdlutils().images().at("placeHolder"),
    };
    factory_->setLayer(ecs::layer::BACKGROUND);
    endImage = factory_->createMultiTextureImage(Vector2D(0, 0), Vector2D(600, 600), texturesArray);
    npcId = 0;
    endText = factory_->createLabel(Vector2D(300, 900),1000,
        endTexts[generalData().personajeToString((Personaje)npcId)][generalData().getNPCData((Personaje)npcId)->felicidad], 
        50);
}



void EndGameScene::init()
{
    //TODO: añadir un contador para que sea necesario leer un mínimo
    if (ih().mouseButtonDownEvent()) {
        nextEnding();
    }

}

void EndGameScene::nextEnding()
{
    //endImage->getComponent<RenderImage>();

    //TODO: hacer un metodo en la factory para crear texturas a partir de texto
    endText->addComponent<RenderImage>();
}

void EndGameScene::loadEnd(Personaje npc, JSONObject& root)
{
    std::string charac = generalData().personajeToString(npc);
    JSONValue* jsonEntry = nullptr;
    jsonEntry = root[charac];
    if (jsonEntry != nullptr)
    {
        int i = 0;
        for (auto& value : jsonEntry->AsArray())
        {
            if (value->IsString()) {
                endTexts[charac][i] = value->AsString();
                //añadir cada linea segun su felicidad de el personaje inidcado
            }
            else {
                throw std::runtime_error("Valor no string en el array");
            }
            i++;
        }
    }
    else
    {
        throw std::runtime_error("Fallo en la carga de dialogo");
    }
}
