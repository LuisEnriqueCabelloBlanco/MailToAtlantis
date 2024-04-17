#include "EndGameScene.h"
#include "../sistemas/ComonObjectsFactory.h"
#include <vector>
#include <sistemas/Felicidad.h>
#include <json/JSON.h>
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

    loadEnd("Pancracio", root);
    
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
}



void EndGameScene::init()
{
	
}

void EndGameScene::loadEnd(const std::string& npc, JSONObject& root)
{
    JSONValue* jsonEntry = nullptr;
    jsonEntry = root[npc];
    if (jsonEntry != nullptr)
    {
        int i = 0;
        for (auto& value : jsonEntry->AsArray())
        {
            if (value->IsString()) {
                endTexts[npc][i] = value->AsString();
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
