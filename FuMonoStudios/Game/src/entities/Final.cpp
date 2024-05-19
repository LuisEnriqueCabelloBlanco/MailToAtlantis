#include "Final.h"
#include <json/JSON.h>
#include <unordered_map>
#include <string>
#include <components/Render.h>
#include <sistemas/ComonObjectsFactory.h>
#include <sdlutils/SDLUtils.h>

std::unordered_map<Personaje, std::unordered_map<Felicidad, std::string>> Final::endTexts_;

Final::Final(ComonObjectsFactory* factory): factory_(factory)
{
    // Comprobamos si hay que inicializar endTexts_
    if (endTexts_.empty()) {
        inicializarFinal();
    }
    // setLayer a UI e inicializacion de escene_
    factory->setLayer(ecs::layer::UI);

    // Creamos entidad periodico
    Texture* periodicoTex = &sdlutils().images().at("periodico");
    periodico_ = factory->createImage(Vector2D(30, 110), Vector2D(periodicoTex->width(), periodicoTex->height()), periodicoTex);
    Transform* periodicoTr = periodico_->getComponent<Transform>();
    periodicoTr->setScale(1);

    // Creamos entidad imagenNpc
    Texture* imagenNpcTex = gD().personajeToTexture(npc::Vagabundo);
    imagenNpc_ = factory->createImage(Vector2D(30, 110), Vector2D(imagenNpcTex->width(), imagenNpcTex->height()), imagenNpcTex);
    Transform* imagenNpcTr = imagenNpc_->getComponent<Transform>();
    imagenNpcTr->setScale(0.25);
    imagenNpcTr->setPos(300, 500);
    //Creamos Texto
    texto_ = factory->createImage(Vector2D(1000, 400), Vector2D(400, 200), nullptr);
}

Final::~Final()
{
    periodico_->setAlive(false);
    imagenNpc_->setAlive(false);
    texto_->setAlive(false);
}

void Final::inicializarFinal()
{
    std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile("recursos/data/ends.json"));

    // check it was loaded correctly
    // the root must be a JSON object
    if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
        throw "Something went wrong while load/parsing dialogues";
    }
    // Inicializamos root_
    JSONObject root = jValueRoot->AsObject();

    // Numero de personajes con finales
    int numNpc = 7;

    // Cargamos endTexts_ HACER METODO STATICO PARA RELLENAR unordered_map
    for (int i = 0; i < numNpc; i++) {
        Personaje npc = (Personaje)i;

        std::string charac = gD().personajeToString(npc);
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
            
            auto finalBien = data.find("FinalBien");
            if (finalBien != data.end())
                endTexts_[npc][FinalBien] = data["FinalBien"]->AsString();
            auto finalMal = data.find("FinalMal");
            if (finalMal != data.end())
                endTexts_[npc][FinalMal] = data["FinalMal"]->AsString();

            endTexts_[npc][NoHabladoAun] = "No hablaste con este Personaje";
        }
        else
        {
            throw std::runtime_error("Fallo en la carga de dialogo");
        }
    }
}

void Final::loadFinal(Personaje npc, Felicidad felicidad)
{    
    // Actualizamos la imagenNpc
    Texture* imagenNpcTex = gD().personajeToTexture(npc);
    imagenNpc_ = factory_->createImage(Vector2D(300, 500), Vector2D(imagenNpcTex->width(), imagenNpcTex->height()), imagenNpcTex);
    Transform* imagenNpcTr = imagenNpc_->getComponent<Transform>();

    //A continuacion el autoescalado y autoposicionamiento de la imagen del npc en el periodico

    // Primero nos quedamos con el width o height dependiendo de que sea el mas grande
    int maxSize = std::max(imagenNpcTex->width(), imagenNpcTex->height());

    // scaleFactor vendra determinado por maxSize
    float scaleFactor = 300.0f / maxSize;
    imagenNpcTr->setScale(scaleFactor);

    // Obtenemos el tamaño de la imagen escalada
    int scaledWidth = imagenNpcTex->width() * scaleFactor;
    int scaledHeight = imagenNpcTex->height() * scaleFactor;

    // Centramos
    int xPos = 500 - scaledWidth / 2;
    int yPos = 650 - scaledHeight / 2;
    imagenNpcTr->setPos(xPos, yPos);

    // Generamos texto
    std::string texto = endTexts_[npc][felicidad];
    factory_->setFont("simpleHandmade");
    if (texto.size() < 450) {
        ecs::Entity* textoEnt = factory_->createLabel(Vector2D(1000, 300), 550, texto, 40, build_sdlcolor(0x000000ff));
    }
    else if (texto.size() < 800) {
        ecs::Entity* textoEnt = factory_->createLabel(Vector2D(1000, 300), 550, texto, 30, build_sdlcolor(0x000000ff));
    }
    else {
        ecs::Entity* textoEnt = factory_->createLabel(Vector2D(1000, 300), 550, texto, 20, build_sdlcolor(0x000000ff));
    }
}


std::string Final::getFinal(Personaje npc, Felicidad nivelFelicidad) {
    return endTexts_[npc][nivelFelicidad];
}

void Final::setActive(bool value)
{
    periodico_->setActive(value);
    imagenNpc_->setActive(value);
    texto_->setActive(value);
}
