#include "WorkRestrictionsSystem.h"

WorkRestrictionsSystem::WorkRestrictionsSystem() : jsonPath(""), numEvents(28)
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
    auto& rand = sdlutils().rand();
    int selection = rand.nextInt(0, numEvents); //cambiar el segundo valor por el ultimo evento que haya en el json + 1 (el random es exclusivo del segundo numero)
    tb::WorkEvent event = getEvent(selection);

    while(!isEventValid(event)) //mientras el evento no sea valido para ese dia (por contener mas mecanicas o distritos de los desbloqueados actualmente) se hace reroll del evento
    {
        selection = rand.nextInt(0, numEvents);
        event = getEvent(selection);
    }

    return event;
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

    event.validDay = jObject["validDay"]->AsNumber();
    event.dialogue = jObject["dialogo"]->AsString();
    event.id = (tb::restrictionId)jObject["id"]->AsNumber();

    switch (event.id)
    {
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

void WorkRestrictionsSystem::eventSwapPipe(WorkEvent& event, JSONObject jObject)
{
    event.swap_pipe_data.targetPipe = (Distrito)generalData().fromStringToDistrito(jObject["target"]->AsString());
    SwappedPipe aux;
    aux.swapActive = jObject["swapActive"]->AsBool();
    aux.originalDis = jObject["originalDis"]->AsBool();
    aux.changedDis = (Distrito)generalData().fromStringToDistrito(jObject["dest"]->AsString());
    event.swap_pipe_data.blockedPipe = (Distrito)generalData().fromStringToDistrito(jObject["blocked"]->AsString());
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

bool WorkRestrictionsSystem::isEventValid(WorkEvent& event)
{
    return generalData().getDay() >= event.validDay;
}
