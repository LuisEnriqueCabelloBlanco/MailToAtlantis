#include "WorkRestrictionsSystem.h"

WorkRestrictionsSystem::WorkRestrictionsSystem() : jsonPath(""), numEvents(28)
{

}

WorkRestrictionsSystem::~WorkRestrictionsSystem()
{

}

void WorkRestrictionsSystem::init()
{
    jsonPath = BOSS_EVENTS_PATH;
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
        throw config_File_Missing(jsonPath);
    }
    // we know the root is JSONObject
    JSONObject root = jValueRoot->AsObject();
    JSONValue* jsonEntry = nullptr;

    jsonEntry = root[eventSelection];
    JSONObject jObject = jsonEntry->AsObject();

    //guardamos en el evento los parametros comunes a todos los tipos de eventos
    event.validDay = jObject["validDay"]->AsNumber();
    ///<summary>
    /// Soy consciente de que podriamos haber hecho un sistema de plantillas con strings que se rellenasen automaticamente acorde a los parametros
    /// del evento y que ademas seria relativamente simple.
    /// Teniendo en cuenta la cantidad de cosas que tengo que hacer y que cuando me di cuenta de que era mas sencillo de lo que pensaba ya tenia
    /// hecho esto, pues asi lo deje (como justificacion extra, esta forma te deja que los dialogos de los eventos sean todos completamente
    /// diferentes unos de otros (aunque ahora mismo todos siguen la misma plantilla))
    ///</summary>
    event.dialogue = jObject["dialogo"]->AsString();
    event.id = (tb::restrictionId)jObject["id"]->AsNumber();

    //en base al tipo de evento rellenamos unos parametros u otros
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

void WorkRestrictionsSystem::eventSwapPipe(WorkEvent& event, JSONObject& jObject)
{
    event.swap_pipe_data.targetPipe = (Distrito)gD().fromStringToDistrito(jObject["target"]->AsString());
    SwappedPipe aux;
    aux.swapActive = jObject["swapActive"]->AsBool();
    aux.originalDis = jObject["originalDis"]->AsBool();
    aux.changedDis = (Distrito)gD().fromStringToDistrito(jObject["dest"]->AsString());
    event.swap_pipe_data.blockedPipe = (Distrito)gD().fromStringToDistrito(jObject["blocked"]->AsString());
    event.swap_pipe_data.dest = aux;
}

void WorkRestrictionsSystem::eventBanType(WorkEvent& event, JSONObject& jObject)
{
    event.ban_type_pipe_data.targetPipe = (Distrito)gD().fromStringToDistrito(jObject["target"]->AsString());
    event.ban_type_pipe_data.ban = gD().stringToTipoPaquete(jObject["ban"]->AsString());
}

void WorkRestrictionsSystem::eventWeightRes(WorkEvent& event, JSONObject& jObject)
{
    event.weight_res_pipe_data.targetPipe = (Distrito)gD().fromStringToDistrito(jObject["target"]->AsString());
    WeightRestriction auxW;
    auxW.weightRestricted = jObject["weightRestricted"]->AsBool();
    auxW.singleType = jObject["singleType"]->AsBool();
    auxW.typeToWeight = gD().stringToTipoPaquete(jObject["typeToWeight"]->AsString());
    auxW.minOrMax = jObject["minOrMax"]->AsNumber();
    auxW.x = gD().stringToNivelPeso(jObject["peso"]->AsString());
    event.weight_res_pipe_data.restrictions = auxW;
}

bool WorkRestrictionsSystem::isEventValid(WorkEvent& event)
{
    return gD().getDay() >= event.validDay; //Devolvemos si un evento es valido para el dia en el que estamos comparando el dia actual con el dia a partir del cual un evento se puede usar, si es menor es que no podemos hacer ese evento (por falta de una mecanica o de un distrito desbloqueado). 
}
