#pragma once
#include <sistemas/PipeManager.h>
#include <json/JSON.h>


using namespace tb;

class WorkRestrictionsSystem
{
public:
    WorkRestrictionsSystem();
    ~WorkRestrictionsSystem();
    void init();

    tb::WorkEvent getRandomEvent();
    tb::WorkEvent getEvent(int selection);
private:
    std::string jsonPath; //ruta del json
    int numEvents; //numero de eventos que hay en el json

    //metodos para leer del json los parametros de cada tipo de evento
    void eventSwapPipe(WorkEvent& event, JSONObject jsonObj);
    void eventBanType(WorkEvent& event, JSONObject jsonObj);
    void eventWeightRes(WorkEvent& event, JSONObject jsonObj);

    //metodo para comprobar si el evento es valido en este dia y si no hacer reroll (hay eventos que utilizan mecanicas desbloqueadas mas tarde como por ejemplo el peso o los distritos)
    bool isEventValid(WorkEvent& event);
};

