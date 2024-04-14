// dialog_manager.cpp
#include "DialogManager.h"
#include <fstream>

#include "DelayedCallback.h"
#include "DialogComponent.h"
#include "Render.h"
#include "../json/JSON.h"
#include "../json/JSONValue.h"
#include "sistemas/ComonObjectsFactory.h"

DialogManager::DialogManager() : currentDialogIndex_(0)
{

}

void DialogManager::init(ecs::Scene* scene)
{
    Vector2D pos = Vector2D(100, LOGICAL_RENDER_HEITH - 250);
    Vector2D size = Vector2D(LOGICAL_RENDER_WIDTH - 100, 200);
    scene->getFactory()->setLayer(ecs::layer::UI);
    boxBackground = scene->getFactory()->createImage(pos, size, &sdlutils().images().at("cuadroDialogo"));
    boxBackground->setActive(false);

    textDialogue = scene->addEntity(ecs::layer::UI);
    auto textTr = textDialogue->addComponent<Transform>(100, 40, 80, 100);
    textTr->setParent(boxBackground->getComponent<Transform>());
    textDialogue->addComponent<RenderImage>();
}

std::string DialogManager::getCurrentDialog() {
    if (currentDialogIndex_ < dialogs_.size()) {
        return dialogs_[currentDialogIndex_];
    }
    else {
        return ""; // No hay m�s di�logos. cleon: ultramegamaxi MAL
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

    std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile("recursos/data/dialogos.json"));

    // check it was loaded correctly
    // the root must be a JSON object
    if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
        throw "Something went wrong while load/parsing dialogues";
    }
    // we know the root is JSONObject
    JSONObject root = jValueRoot->AsObject();
    JSONValue* jsonEntry = nullptr;

    const std::string& stringDialogSel = dialogSelectionToString(ds);

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

void DialogManager::closeConversation()
{
    //boxBackground->setActive(false);
    //textDialogue->setActive(false);
    //textDialogue->removeComponent<DialogComponent>();
    //textDialogue->addComponent<DelayedCallback>(0.1, [this]() {
    //    canStartConversation = true;
    //    });
    textDialogue->getComponent<RenderImage>()->setTexture(nullptr);
    textDialogue->removeComponent<DialogComponent>();
    boxBackground->getComponent<RenderImage>()->setTexture(nullptr);
    textDialogue->addComponent<DelayedCallback>(0.1, [this]() {
        canStartConversation = true;
        });
}

void DialogManager::fixText(std::string& text)
{
    crearTildes(text);
}

void DialogManager::crearTildes(std::string& aux)
{
    size_t pos = 0;
    while ((pos = aux.find('$', pos)) != std::string::npos) {
        if (pos > 0) {
            std::string acento;
            switch (aux[pos + 1]) {
                case 'a':
                    acento = "á";
                    break;
                case 'e':
                    acento = "é";
                    break;
                case 'i':
                    acento = "í";
                    break;
                case 'o':
                    acento = "ó";
                    break;
                case 'u':
                    acento = "ú";
                    break;
                case 'A':
                    acento = "Á";
                    break;
                case 'E':
                    acento = "É";
                    break;
                case 'I':
                    acento = "Í";
                    break;
                case 'O':
                    acento = "Ó";
                    break;
                case 'U':
                    acento = "Ú";
                    break;
            }
            aux.replace(pos, 2, acento);  // Reemplazar el carácter anterior y el $
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
    case JefeOficina:
        aux = "JefeOficina";
        break;
    case Tutorial:
        aux = "Tutorial";
        break;
    case BryantMyers:
        aux = "EsclavaRemix";
        break;
    }
    return aux;
}

bool DialogManager::isNPC(const DialogSelection ds)
{
    return ds < 7;
}