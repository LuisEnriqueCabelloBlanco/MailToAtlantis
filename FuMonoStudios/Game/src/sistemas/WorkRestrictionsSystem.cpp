#include "WorkRestrictionsSystem.h"

WorkRestrictionsSystem::WorkRestrictionsSystem() : jsonPath("")
{

}

WorkRestrictionsSystem::~WorkRestrictionsSystem()
{

}

void WorkRestrictionsSystem::init()
{
    jsonPath = "recursos/data/eventosjefe.json";
}

tb::WorkEvent WorkRestrictionsSystem::getRandomEvent()
{
    return getEvent(4);
}

tb::WorkEvent WorkRestrictionsSystem::getEvent(int selection)
{
    tb::WorkEvent event; 
    std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile(jsonPath));
    std::string eventSelection = "Evento" + std::to_string(selection);
    // check it was loaded correctly
    // the root must be a JSON object
    if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
        throw "Something went wrong while load/parsing dialogues";
    }
    // we know the root is JSONObject
    JSONObject root = jValueRoot->AsObject();
    JSONValue* jsonEntry = nullptr;

    jsonEntry = root[eventSelection];
    JSONObject jObject = jsonEntry->AsObject();

    event.dialogue = jObject["dialogo"]->AsString();
    event.id = (tb::restrictionId)jObject["id"]->AsNumber();

    switch (event.id)
    {
        case BLOCK_PIPE:
            eventBlockPipe(event, jObject);
            break;
        case SWAP_PIPE:
            eventSwapPipe(event, jObject);
            break;
        case BAN_TYPE_IN_PIPE:
            eventBanType(event, jObject);
            break;
        case WEIGHT_RESTRICT_PIPE:
            eventWeightRes(event, jObject);
            break;
    }

    return event;
}

void WorkRestrictionsSystem::eventBlockPipe(WorkEvent& event, JSONObject jObject)
{
    event.block_pipe_data.targetPipe = (Distrito)generalData().fromStringToDistrito(jObject["target"]->AsString());
}

void WorkRestrictionsSystem::eventSwapPipe(WorkEvent& event, JSONObject jObject)
{
    event.swap_pipe_data.targetPipe = (Distrito)generalData().fromStringToDistrito(jObject["target"]->AsString());
    SwappedPipe aux;
    aux.swapActive = jObject["swapActive"]->AsBool();
    aux.originalDis = jObject["originalDis"]->AsBool();
    aux.changedDis = (Distrito)generalData().fromStringToDistrito(jObject["dest"]->AsString());
    event.swap_pipe_data.dest = aux;
}

void WorkRestrictionsSystem::eventBanType(WorkEvent& event, JSONObject jObject)
{
    event.ban_type_pipe_data.targetPipe = (Distrito)generalData().fromStringToDistrito(jObject["target"]->AsString());
    event.ban_type_pipe_data.ban = generalData().stringToTipoPaquete(jObject["ban"]->AsString());
}

void WorkRestrictionsSystem::eventWeightRes(WorkEvent& event, JSONObject jObject)
{
    event.weight_res_pipe_data.targetPipe = (Distrito)generalData().fromStringToDistrito(jObject["target"]->AsString());
    WeightRestriction auxW;
    auxW.weightRestricted = jObject["weightRestricted"]->AsBool();
    auxW.singleType = jObject["singleType"]->AsBool();
    auxW.typeToWeight = generalData().stringToTipoPaquete(jObject["typeToWeight"]->AsString());
    auxW.minOrMax = jObject["minOrMax"]->AsNumber();
    auxW.x = generalData().stringToNivelPeso(jObject["peso"]->AsString());
    event.weight_res_pipe_data.restrictions = auxW;
}
