#include "Final.h"
#include <json/JSON.h>
#include <unordered_map>
#include <string>

#include "sistemas/ComonObjectsFactory.h"
#include "sdlutils/SDLUtils.h"

std::unordered_map<Personaje, std::unordered_map<Felicidad, std::string>> Final::endTexts_;

Final::Final(ecs::Scene* escene, Personaje npc, Felicidad felicidad)
{
    // Comprobamos si hay que inicializar endTexts_
    if (endTexts_.empty()) {
        inicializarFinal(escene, npc, felicidad);
    }

    // Obtenemos el texto del final pedido
    std::string texto = endTexts_[npc][felicidad];

    // setLayer a UI e inicializacion de escene_
    ComonObjectsFactory* factory = escene->getFactory();
    factory->setLayer(ecs::layer::UI);
    escene_ = escene;

    // Creamos entidad periodico
    Texture* periodicoTex = &sdlutils().images().at("periodico");
    periodico_ = factory->createImage(Vector2D(30, 110), Vector2D(periodicoTex->width(), periodicoTex->height()), periodicoTex);
    Transform* periodicoTr = periodico_->getComponent<Transform>();
    periodicoTr->setScale(1);

    // Creamos entidad imagenNpc
    Texture* imagenNpcTex = gD().personajeToTexture(npc);
    imagenNpc_ = factory->createImage(Vector2D(30, 110), Vector2D(imagenNpcTex->width(), imagenNpcTex->height()), imagenNpcTex);
    Transform* imagenNpcTr = imagenNpc_->getComponent<Transform>();
    imagenNpcTr->setScale(0.25);
    imagenNpcTr->setPos(300, 500);

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
    std::string textoInicial = endTexts_[npc][felicidad];
    factory->createLabel(Vector2D(1000, 400), Vector2D(400, 200), texto, 50);   
}

Final::~Final()
{
    delete periodico_;
    delete imagenNpc_;
    delete texto_;
}

void Final::inicializarFinal(ecs::Scene* escene, Personaje npc, Felicidad felicidad)
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
    ComonObjectsFactory* factory = escene_->getFactory();
    factory->setLayer(ecs::layer::UI);

    // Borramos la imagen anterior
    imagenNpc_ = nullptr;
    delete imagenNpc_;

    // Actualizamos la imagenNpc
    Texture* imagenNpcTex = gD().personajeToTexture(npc);
    imagenNpc_ = factory->createImage(Vector2D(300, 500), Vector2D(imagenNpcTex->width(), imagenNpcTex->height()), imagenNpcTex);
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
    factory->createLabel(Vector2D(1000, 400), Vector2D(400, 200), texto, 50);
}

std::string Final::getFinal(Personaje npc, Felicidad nivelFelicidad) {
    return endTexts_[npc][nivelFelicidad];
}
