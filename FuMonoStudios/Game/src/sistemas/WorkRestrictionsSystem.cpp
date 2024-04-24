#include "WorkRestrictionsSystem.h"

#include "json/JSON.h"

WorkRestrictionsSystem::WorkRestrictionsSystem() : jsonPath("")
{

}

WorkRestrictionsSystem::~WorkRestrictionsSystem()
{

}

void WorkRestrictionsSystem::init()
{
    jsonPath = "recursos/data/dialogos.json";
}

tb::WorkEvent WorkRestrictionsSystem::getEvent(GeneralData::DialogSelection ds, int iteration)
{
    tb::WorkEvent event; 

    std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile(jsonPath));

    // check it was loaded correctly
    // the root must be a JSON object
    if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
        throw "Something went wrong while load/parsing dialogues";
    }
    // we know the root is JSONObject
    JSONObject root = jValueRoot->AsObject();
    JSONValue* jsonEntry = nullptr;

    const std::string& stringDialogSel = generalData().dialogSelectionToString(ds);
    jsonEntry = root[stringDialogSel];

    return event;
}
