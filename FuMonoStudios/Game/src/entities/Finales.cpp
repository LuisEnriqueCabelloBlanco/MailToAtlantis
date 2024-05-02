#include "Finales.h"
#include <json/JSON.h>
#include "sistemas/ComonObjectsFactory.h"
#include "sdlutils/SDLUtils.h"
#include "architecture/GeneralData.h"

Finales::Finales()
{
    std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile("recursos/data/ends.json"));

    // check it was loaded correctly
    // the root must be a JSON object
    if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
        throw "Something went wrong while load/parsing dialogues";
    }
    // Inicializamos root_
    JSONObject root = jValueRoot->AsObject();

    // Obtenemos el numero de personajes
    int numNpc = std::numeric_limits<enum Personaje>::max();

    // Cargamos endTexts_
    for (int i = 0; i < numNpc; i++) {
        Personaje npc = static_cast<Personaje>(i);

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
}

void Finales::inicializarFinal(ecs::Scene escene, Personaje npc, Felicidad felicidad)
{
    std::string texto = endTexts_[npc][felicidad];

    ComonObjectsFactory* factory = escene.getFactory();

    factory->setLayer(ecs::layer::UI);

    // Entidad periodico
    Texture* periodicoTex = &sdlutils().images().at("periodico");
    periodico_ = factory->createImage(Vector2D(30, 110), Vector2D(periodicoTex->width(), periodicoTex->height()), periodicoTex);
    Transform* periodicoTr = periodico_->getComponent<Transform>();
    periodicoTr->setScale(0.5);
    periodicoTr->setPos(100, 100);

    // Entidad imagenNpc
    Texture* imagenNpcTex = generalData().personajeToTexture(npc);
    imagenNpc_ = factory->createImage(Vector2D(30, 110), Vector2D(imagenNpcTex->width(), imagenNpcTex->height()), imagenNpcTex);
    
    // Generamos Dialogo
}

void Finales::updateFinal(ecs::Scene escene, Personaje npc, Felicidad felicidad)
{
    if (periodico_ != nullptr) {
        std::string texto = endTexts_[npc][felicidad];

        ComonObjectsFactory* factory = escene.getFactory();
        factory->setLayer(ecs::layer::UI);

        // Actualizamos la imagenNpc
        Texture* imagenNpcTex = generalData().personajeToTexture(npc);
        imagenNpc_ = factory->createImage(Vector2D(30, 110), Vector2D(imagenNpcTex->width(), imagenNpcTex->height()), imagenNpcTex);
        
        // Generamos dialogo
    }
}

void Finales::deleteFinal()
{
    delete periodico_;
    delete imagenNpc_;
}

std::string Finales::getFinal(Personaje npc, Felicidad nivelFelicidad) {
    return endTexts_[npc][Minima];
}
