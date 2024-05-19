#pragma once
#include <sistemas/PipeManager.h>


using namespace tb;

class WorkRestrictionsSystem
{
public:
    WorkRestrictionsSystem();
    ~WorkRestrictionsSystem();
    void init();

    ///<summary>
    ///devuelve un evento de trabajo aleatorio 
    ///</summary>
    tb::WorkEvent getRandomEvent();

    ///<summary>
    ///devuelve el evento de trabajo del json de "eventosJefe.json" con el indice correspondiente
    ///</summary>
    tb::WorkEvent getEvent(int selection);
private:
    std::string jsonPath; //ruta del json
    int numEvents; //numero de eventos que hay en el json

    ///<summary>
    ///lee parametros del evento de cambio de tuberias
    ///</summary>
    void eventSwapPipe(WorkEvent& event, JSONObject& jsonObj);

    ///<summary>
    ///lee parametros del evento de prohibicion de tipos de paquetes
    ///</summary>
    void eventBanType(WorkEvent& event, JSONObject& jsonObj);

    ///<summary>
    ///lee parametros del evento de restriccion de peso
    ///</summary>
    void eventWeightRes(WorkEvent& event, JSONObject& jsonObj);

    ///<summary>
    ///metodo para comprobar si el evento es valido en este dia y si no hacer reroll (hay eventos que utilizan mecanicas desbloqueadas mas tarde como por ejemplo el peso o los distritos)
    ///</summary>
    bool isEventValid(WorkEvent& event);
};

