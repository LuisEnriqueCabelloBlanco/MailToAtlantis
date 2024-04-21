#include "EndGameScene.h"
#include "../sistemas/ComonObjectsFactory.h"
#include <vector>
#include <json/JSON.h>
#include <architecture/GeneralData.h>
#include <string>
#include <sdlutils/InputHandler.h>
#include <components/Render.h>
#include <architecture/Game.h>

EndGameScene::EndGameScene()
{
    std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile("recursos/data/ends.json"));

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
    
  /*  std::vector<Texture*> texturesArray = {
        &sdlutils().images().at("placeHolder"),
        &sdlutils().images().at("placeHolder"),
        &sdlutils().images().at("placeHolder"),
        &sdlutils().images().at("placeHolder"),
        &sdlutils().images().at("placeHolder"),
        &sdlutils().images().at("placeHolder"),
        &sdlutils().images().at("placeHolder"),
    };
    factory_->setLayer(ecs::layer::BACKGROUND);
    endImage_ = factory_->createMultiTextureImage(Vector2D(0, 0), Vector2D(600, 600), texturesArray);*/
   
}



void EndGameScene::init()
{
    npcId_ = 0;
    endText_ = factory_->createLabel(Vector2D(300, 900), 1000,
        endTexts_[(Personaje)npcId_][generalData().getNPCData((Personaje)npcId_)->felicidad], 
        50,build_sdlcolor(0xFFFFFFFF));
}

void EndGameScene::update()
{
    //TODO: anadir un contador para que sea necesario leer un mínimo
    if (ih().mouseButtonDownEvent() ) {
        if (npcId_ < 6) {
            nextEnding();
        }
        else {
            gm().requestChangeScene(ecs::sc::END_SCENE, ecs::sc::MENU_SCENE);
        }
    }
}



void EndGameScene::nextEnding()
{
    npcId_++;
    Personaje npc = (Personaje)npcId_;
    //endImage->getComponent<RenderImage>();
    Texture* endText = factory_->createTextTexture(endTexts_[npc][generalData().getNPCData(npc)->felicidad], 50, build_sdlcolor(0xFFFFFFFF));
    endText_->getComponent<RenderImage>()->setTexture(endText);
    endText_->getComponent<Transform>()->setWidth(endText->width());
}

void EndGameScene::loadEnd(Personaje npc, JSONObject& root)
{
    std::string charac = generalData().personajeToString(npc);
    JSONValue* jsonEntry = nullptr;
    jsonEntry = root[charac];
    if (jsonEntry != nullptr)
    {
        auto data = jsonEntry->AsObject();
        endTexts_[npc][Minima] = data["Mini"]->AsString();
        endTexts_[npc][Mala] = data["Mala"]->AsString();
        endTexts_[npc][Normal] = data["Normal"]->AsString();
        endTexts_[npc][Buena] = data["Buena"]->AsString();
        endTexts_[npc][Maxima] = data["Maxima"]->AsString();
        endTexts_[npc][NoHabladoAun] = "No hablaste con este Personaje";
    }
    else
    {
        throw std::runtime_error("Fallo en la carga de dialogo");
    }
}
