// dialog_manager.cpp
#include <utils/checkML.h>
#include "DialogManager.h"
#include <fstream>

#include "DelayedCallback.h"
#include "DialogComponent.h"
#include "Render.h"
#include "../json/JSON.h"
#include "../json/JSONValue.h"
#include "QATools/DataCollector.h"
#include "sistemas/ComonObjectsFactory.h"

DialogManager::DialogManager() : scene_(nullptr), currentDialogIndex_(0),boxBackground(nullptr), textDialogue(nullptr), endDialogueCallback(nullptr)
{

}

DialogManager::~DialogManager()
{
}


void DialogManager::init(ecs::Scene* scene)
{
    init(scene, "recursos/data/dialogos.json");
}

void DialogManager::init(ecs::Scene* scene, const std::string& path) {

    scene_ = scene;

    jsonPath = path;

    canStartConversation = true;

    dialogueCooldownTime = 100;
    timer_ = sdlutils().virtualTimer().currTime();
    dialogueCooldown = dialogueCooldownTime + sdlutils().virtualTimer().currTime();
    controlTimer = false;
}

void DialogManager::update()
{
    if(controlTimer)
    {
        timer_ = sdlutils().virtualTimer().currTime();
        if(timer_ > dialogueCooldown)
        {
            canStartConversation = true;
            controlTimer = false;
        }
    }
}

std::string DialogManager::getCurrentDialog() {
    if (currentDialogIndex_ < dialogs_.size()) {
        return dialogs_[currentDialogIndex_];
    }
    else {
        return " "; // No hay m�s di�logos. cleon: ultramegamaxi MAL
    }
}


bool DialogManager::nextDialog() {

    bool isEndOfConversation = (currentDialogIndex_ >= dialogs_.size() - 1);

    if (isEndOfConversation) {

        currentDialogIndex_ = 0;
    }
    else {
        currentDialogIndex_++;
    }
    return isEndOfConversation;
}

void DialogManager::setDialogues(const DialogSelection ds, const std::string& tipoDialogo, int dialogueIteration)
{
    //eliminamos los dialogos anteriores
    dialogs_.clear();
    //reseteamos la posicon del indice
    currentDialogIndex_ = 0;

    std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile(jsonPath));

    // check it was loaded correctly
    // the root must be a JSON object
    if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
        throw "Something went wrong while load/parsing dialogues";
    }
    // we know the root is JSONObject
    JSONObject root = jValueRoot->AsObject();
    JSONValue* jsonEntry = nullptr;

    const std::string& stringDialogSel = dialogSelectionToString(ds);
    auto a =generalData().getDay();
    jsonEntry = root[stringDialogSel];
    if (jsonEntry != nullptr)
    {
        std::string strDialogo = tipoDialogo;

        // Si hemos elegido un setDialogue tipo 3
        if (strDialogo == "NULL")
        {
            if (!jsonEntry->IsArray())
                throw std::runtime_error("No es array el dialogo: " + stringDialogSel);

            for (auto& value : jsonEntry->AsArray())
            {
                if (value->IsString()) {
                    std::string valueText = value->AsString();

                    fixText(valueText);

                    dialogs_.push_back(valueText);
                }
                else {
                    throw std::runtime_error("Valor no string en el array: " + stringDialogSel);
                }
            }
        }
        // si hemos elegido un setDialogue tipo 2
        else if (dialogueIteration == -1)
        {
            JSONObject jsonEntryObj = jsonEntry->AsObject();
            // Accede al tipo de diálogo específico
            jsonEntry = jsonEntryObj[strDialogo];

            if (!jsonEntry->IsArray())
                throw std::runtime_error("No es array el dialogo: " + stringDialogSel);

            for (auto& value : jsonEntry->AsArray())
            {
                if (value->IsString()) {
                    std::string valueText = value->AsString();

                    fixText(valueText);

                    dialogs_.push_back(valueText);
                }
                else {
                    throw std::runtime_error("Valor no string en el array: " + stringDialogSel);
                }
            }
        }
        // si hemos elegido setDialogue tipo 3 (como tipo 2 pero iterable)
        else
        {
            JSONObject jsonEntryObj = jsonEntry->AsObject();
            // Accede al tipo de diálogo específico
            jsonEntry = jsonEntryObj[strDialogo];

            jsonEntryObj = jsonEntry->AsObject();

            jsonEntry = jsonEntryObj[std::to_string(dialogueIteration)];

            for (auto& value : jsonEntry->AsArray())
            {
                if (value->IsString()) {
                    std::string valueText = value->AsString();

                    fixText(valueText);

                    dialogs_.push_back(valueText);
                }
                else {
                    throw std::runtime_error("Valor no string en el array: " + stringDialogSel);
                }
            }

            if (isNPC(ds))
            {
                generalData().getNPCData(generalData().stringToPersonaje(dialogSelectionToString(ds)))->iterateDialogues();
            }
        }
    }
    else
    {
        throw std::runtime_error("Fallo en la carga de dialogo: " + stringDialogSel);
    }
}

void DialogManager::setDialogues(std::string& dialogo) //mirar en el .h por que no es const juro que tiene sentido
{
    dialogs_.clear();

    fixText(dialogo);

    dialogs_.push_back(dialogo);
}

void DialogManager::startConversation(const std::string& character)
{
    if(canStartConversation)
    {
        auto charac = generalData().stringToPersonaje(character); //de que personaje queremos el dialogo
        auto data = generalData().getNPCData(charac); //data de dicho personaje

        // activamos los dialogos correspondientes
        std::pair<const std::string, int> aux = data->getDialogueInfo(); 


        setDialogues((DialogSelection)generalData().stringToPersonaje(character), aux.first, aux.second);

        setDialogueEntitiesActive(true);


        std::cout << "jefe otro dialogo que este tenia un agujero\n";
#ifdef QA_TOOLS
        dataCollector().recordNPC(charac + 1, aux.second, generalData().getNPCData(charac)->felicidad);
#endif // QA_TOOLS

        canStartConversation = false;
    }
}

void DialogManager::startConversationWithObj(const std::string& interactableObj)
{
    if (canStartConversation)
    {
        const std::string aux1 = std::to_string(generalData().getDay());

        const std::string aux2 = std::to_string(sdlutils().rand().nextInt(0, 3));

        setDialogues((DialogManager::DialogSelection)(generalData().stringToObjInt(interactableObj) + 10), "Texto"+interactableObj+aux1+aux2);

        setDialogueEntitiesActive(true);

        std::cout << "jefe otro dialogo que este tenia un agujero\n";
        canStartConversation = false;
    }
}

void DialogManager::closeDialogue()
{
    setDialogueEntitiesActive(false);

    timer_ = sdlutils().virtualTimer().currTime();
    dialogueCooldown = sdlutils().virtualTimer().currTime() + dialogueCooldownTime  ;
    controlTimer = true;

    if (endDialogueCallback != nullptr)
        endDialogueCallback();
}

void DialogManager::setDialogueEntitiesActive(bool onoff) //me sigue pareciendo estupido lo de crear y destruir las cosas constantemente cuando las podemos reutilizar pero bueno si lo quereis asi
{
    /*boxBackground->setActive(onoff);
    textDialogue->setActive(onoff);*/

    if(onoff)
    {
        createBox();
        createText();
    }
    else
    {
        boxBackground->setAlive(false);
        textDialogue->setAlive(false);
    }

}

void DialogManager::fixText(std::string& text)
{
  size_t pos = 0;
  while ((pos = text.find('$', pos)) != std::string::npos) {
    if (pos > 0) {
      std::string newChar;
      switch (text[pos + 1]) {
      case 'a':
        newChar = "á";
        break;
      case 'e':
        newChar = "é";
        break;
      case 'i':
        newChar = "í";
        break;
      case 'o':
        newChar = "ó";
        break;
      case 'u':
        newChar = "ú";
        break;
      case 'A':
        newChar = "Á";
        break;
      case 'E':
        newChar = "É";
        break;
      case 'I':
        newChar = "Í";
        break;
      case 'O':
        newChar = "Ó";
        break;
      case 'U':
        newChar = "Ú";
        break;
      case '!':
        newChar = "¡";
        break;
      case '?':
        newChar = "¿";
        break;
      case '%':
        newChar = "ñ";
        break;
      }
      text.replace(pos, 2, newChar);  // Reemplazar el carácter anterior y el $
    }
    pos++;  // Avanzar la posición de búsqueda para evitar un bucle infinito si se encuentra un $
  }
}

std::string DialogManager::dialogSelectionToString(const DialogSelection ds)
{
    std::string aux;
    switch (ds)
    {
    case Vagabundo:
        aux = "Vagabundo";
        break;
    case Secretario:
        aux = "Secretario";
        break;
    case Campesino:
        aux = "Campesino";
        break;
    case Artesano:
        aux = "Artesano";
        break;
    case Tarotisa:
        aux = "Tarotisa";
        break;
    case Soldado:
        aux = "Soldado";
        break;
    case Contable:
        aux = "Contable";
        break;
    case Jefe:
        aux = "Jefe";
        break;
    case Tutorial:
        aux = "Tutorial";
        break;
    case Intro:
        aux = "Intro";
        break;

    //Dialogos objetos distritos
        //Hestia
    case CasaGrande: aux = "CasaGrande"; break;
    case CartelOficina: aux = "CartelOficina"; break;
    case Muro: aux = "Muro"; break;

        //Artemisa
    case TiendaPociones: aux = "TiendaPociones"; break;
    case TiendaBolas: aux = "TiendaBolas"; break;
    case TiendaJarrones: aux = "TiendaJarrones"; break;

        //Demeter
    case Molino: aux = "Molino"; break;
    case Arbol: aux = "Arbol"; break;
    case Carreta: aux = "Carreta"; break;

        //Hefesto
    case PulpoCartel: aux = "PulpoCartel"; break;
    case TiendaCeramica: aux = "TiendaCeramica"; break;
    case TiendaEsculturas: aux = "TiendaEsculturas"; break;

        //Hermes
    case TiendaDerecha: aux = "TiendaDerecha"; break;
    case PanteonIzq: aux = "PanteonIzq"; break;
    case PanteonDer: aux = "PanteonDer"; break;

        //Apolo
    case Panteon: aux = "Panteon"; break;
    case Edificios: aux = "Edificios"; break;
    case Charco: aux = "Charco"; break;

        //Poseidon
    case Casa1: aux = "casa1"; break;
    case Casa2: aux = "casa2"; break;

    //default: break;
    }
    return aux;
}

bool DialogManager::isNPC(const DialogSelection ds)
{
    return ds < 7;
}

void DialogManager::createBox()
{
    Vector2D pos = Vector2D(100, LOGICAL_RENDER_HEITH - 250);
    Vector2D size = Vector2D(LOGICAL_RENDER_WIDTH - 100, 200);
    scene_->getFactory()->setLayer(ecs::layer::UI);
    boxBackground = scene_->getFactory()->createImage(pos, size, &sdlutils().images().at("cuadroDialogo"));
    scene_->getFactory()->setLayer(ecs::layer::DEFAULT);
}

void DialogManager::createText()
{
    textDialogue = scene_->addEntity(ecs::layer::UI);
    auto textTr = textDialogue->addComponent<Transform>(100, 40, 80, 100);
    textTr->setParent(boxBackground->getComponent<Transform>());
    textDialogue->addComponent<RenderImage>();
    textDialogue->addComponent<DialogComponent>(this);
    scene_->getFactory()->setLayer(ecs::layer::DEFAULT);
}
