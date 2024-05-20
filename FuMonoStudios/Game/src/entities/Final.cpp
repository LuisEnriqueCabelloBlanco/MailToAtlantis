#include "Final.h"
#include <json/JSON.h>
#include <unordered_map>
#include <string>
#include <components/Render.h>
#include <sistemas/ComonObjectsFactory.h>
#include <sdlutils/SDLUtils.h>

std::unordered_map<Personaje, std::unordered_map<Felicidad, std::string>> Final::endTexts_;

Final::Final(ComonObjectsFactory* factory, Vector2D npcImagePos, Vector2D textPos): factory_(factory), textoPos_(textPos), imagenNpcPos_(npcImagePos)
{
    // Comprobamos si hay que inicializar endTexts_
    if (endTexts_.empty()) {
        inicializarFinal();
    }
    // setLayer a UI e inicializacion de escene_
    factory->setLayer(ecs::layer::UI);

    // Creamos entidad periodico
    Texture* periodicoTex = &sdlutils().images().at("periodico");
    periodico_ = factory->createImage(imagenNpcPos_ - Vector2D(460,530), Vector2D(periodicoTex->width(), periodicoTex->height()), periodicoTex);
    Transform* periodicoTr = periodico_->getComponent<Transform>();

    // Creamos entidad imagenNpc
    Texture* imagenNpcTex = gD().personajeToTexture(npc::Vagabundo);
    imagenNpc_ = factory->createImage(imagenNpcPos_, Vector2D(imagenNpcTex->width(), imagenNpcTex->height()), imagenNpcTex);
    Transform* imagenNpcTr = imagenNpc_->getComponent<Transform>();
    imagenNpcTr->setScale(0.25);
    //Creamos Texto
    texto_ = factory->createImage(textoPos_, Vector2D(400, 200), nullptr);
}

Final::~Final()
{
    periodico_->setAlive(false);
    imagenNpc_->setAlive(false);
    texto_->setAlive(false);
}

void Final::inicializarFinal()
{
    std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile(END_PATH));

    // check it was loaded correctly
    // the root must be a JSON object
    if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
        throw config_File_Missing(END_PATH);
    }
    // Inicializamos root_
    JSONObject root = jValueRoot->AsObject();

    // Numero de personajes con finales
    int numNpc = 7;

    // Cargamos endTexts_
    for (int i = 0; i < numNpc; i++) {
        Personaje npc = (Personaje)i;

        std::string charac = gD().personajeToString(npc);
        JSONValue* jsonEntry = nullptr;
        jsonEntry = root[charac];
        if (jsonEntry != nullptr)
        {
            auto data = jsonEntry->AsObject();
            endTexts_[npc][SeFue] = data["Mala"]->AsString();
            endTexts_[npc][Minima] = data["Mala"]->AsString();
            endTexts_[npc][Mala] = data["Mala"]->AsString();
            endTexts_[npc][Normal] = data["Normal"]->AsString();
            endTexts_[npc][NoHabladoAun] = data["Normal"]->AsString();
            endTexts_[npc][Buena] = data["Buena"]->AsString();
            endTexts_[npc][Maxima] = data["Maxima"]->AsString();
            
            if (npc == Personaje::Vagabundo || npc == Personaje::Secretario) {
                endTexts_[npc][FinalBien] = data["FinalBien"]->AsString();
                endTexts_[npc][FinalMal] = data["FinalMal"]->AsString();
            }
        }
        else
        {
#ifdef _DEBUG
            std::cerr << wrong_JSON_Format(END_PATH).what() <<std::endl;
#endif // _DEBUG

        }
    }
}

void Final::loadFinal(Personaje npc, Felicidad felicidad)
{    
    // Actualizamos la imagenNpc
    Texture* imagenNpcTex = gD().personajeToTexture(npc);
    imagenNpc_->getComponent<RenderImage>()->setTexture(imagenNpcTex);
    Transform* imagenNpcTr = imagenNpc_->getComponent<Transform>();
    imagenNpcTr->setWidth(imagenNpcTex->width());
    imagenNpcTr->setHeith(imagenNpcTex->height());

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
    int xPos = imagenNpcPos_.getX() - scaledWidth / 2;
    int yPos = imagenNpcPos_.getY() - scaledHeight / 2;
    imagenNpcTr->setPos(xPos, yPos);

    // Generamos texto
    std::string texto = endTexts_[npc][felicidad];
    factory_->setFont("simpleHandmade");
    if (texto.size() < 450) {
        texto_ = factory_->createLabel(textoPos_, 850, texto, 50, build_sdlcolor(0x000000ff));
    }
    else if (texto.size() < 800) {
        texto_ = factory_->createLabel(textoPos_, 850, texto, 40, build_sdlcolor(0x000000ff));
    }
    else {
        texto_ = factory_->createLabel(textoPos_, 850, texto, 30, build_sdlcolor(0x000000ff));
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
