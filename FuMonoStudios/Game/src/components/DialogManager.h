// dialog_manager.h
#pragma once
#include <utils/checkML.h>
#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include "../architecture/GeneralData.h"
#include "../architecture/Scene.h"

/*
- - - - - - - COMO SE USA - - - - - - - - -
- Anadir a la escena un DialogManager
- Llamar en el init de la escena al init del DialogManager
- Llamar en el update de la escena al update del DialogManager
- En la cosa donde quieres un dialogo (por ejemplo la funcion de click en un personaje), llamar a startConversation
- En caso de querer que ocurra algo en especial cuando termine el dialogo llamar a setEndDialogueCallback
- SI NO OS QUEDA ALGUNA COSA CLARA NI DESPUES DE MIRAR EL EJEMPLO DE LA EXPLORATION SCENE POR FAVOR PREGUNTADME (David)
/*
Clase que genera los dialogos que se van a escribir
Carga los dialogos del json dialogos.json

Para cargar una entrada en el json, debemos usar el enum DialogSelection.
Este sistema ir� a�adiendo los strings al dialogComponent.
En caso de ser un npc, tambien hara que sus dialogos cambien, en funcion de
si ya le has hablado etc

Hay 3 tipos de setDialogues en funcion de las necesidades del dialog.json
 
1- Si le pasas un dialogSelection, un tipoDialogo y un int de iteracion
"Eventos": {
      "1": [
        "1evento"
      ],
      "2": [
        "2evento"
      ]
2- Si le pasas un dialogSelection y un tipoDialogo
"Contable": {
    "Presentacion": [
      "hola",
      "hola2"
    ],
3- Si le pasas solo el dialogSelection
"EsclavaRemix": [
    "Bryant myers",
    "Hoy de nuevo te voy a veeer",
  ]

*/

class DialogManager {
public:
    // aqui se elije que rama de dialogo escojer dentro del json
    enum DialogSelection {
        Vagabundo, Secretario, Campesino, Artesano, Tarotisa, Soldado, Contable,
        JefeOficina, Tutorial, BryantMyers,
        CasaGrande, CartelOficina, Muro, //Hestia
        TiendaPociones, TiendaBolas, TiendaJarrones, //Artemisa
        Molino, Arbol, Carreta, //Demeter
        PulpoCartel, TiendaCeramica, TiendaEsculturas, //Hefesto
        TiendaDerecha, PanteonIzq, PanteonDer, //Hermes
        Panteon, Edificios, Charco, //Apolo
        Casa1, Casa2 //Poseidon
    };

    DialogManager();
    ~DialogManager();

    //init que por defecto pone el path del json de los dialogos
    void init(ecs::Scene* scene); 
    //init al que le puedes especificar el path del json del que quieres leer (por ejemplo se usara para el )
    void init(ecs::Scene* scene, const std::string& jsonPath);

    void update();
    /// <summary>
    /// Devuelve el dialogo acutal segun el indice de dialogo
    /// </summary>
    /// <returns></returns>
    std::string getCurrentDialog();

    /// <summary>
    /// Avanza al siguente dialogo. 
    /// Si ya est� en el ultimo di�logo devuelve false y resetea
    /// el index a 0
    /// </summary>
    bool nextDialog();
    
    // Establece los dialogos del tipo introducido mirar arriba para saber cual usar
    void setDialogues(const DialogSelection ds, const std::string& tipoDialogo, int dialogueSelection);
    void setDialogues(const DialogSelection ds, const std::string& t) { setDialogues(ds, t, -1); }
    void setDialogues(const DialogSelection ds) { setDialogues(ds, "NULL", -1); }
    void setDialogues(std::string& dialogo); //no se pasa por const porque la modificamos con el fixText, no te preocupes cleon

    void startConversation(const std::string& character);
    void startConversationWithObj(const std::string& interactableObj);

    //para quitar la caja de texto y el propio texto
    void closeDialogue();

    inline bool getCanStartConversation() { return canStartConversation; }

    //si queremos anadir un callback para que ocurra algo cuando se acaba el dialogo 
    void setEndDialogueCallback(std::function<void()> func) { endDialogueCallback = func; }

    //activar/desactivar caja y texto
    void setDialogueEntitiesActive(bool onoff);

    void fixText(std::string& text);

private:

    std::string dialogSelectionToString(DialogSelection ds);

    bool isNPC(const DialogSelection ds);



    /// <summary>
    /// path del archivo json, si no se especifica por defecto sera el de dialogos
    /// </summary>
    std::string jsonPath;

    /// <summary>
    /// Vector donde se almacenan todos los di�logos que se van a soltar
    /// </summary>
    std::vector<std::string> dialogs_;

    /// <summary>
    /// Indice que indica en que dialogo nos encontramos
    /// </summary>
    size_t currentDialogIndex_;

    /// <summary>
    /// booleano para saber si puedes empezar una conversacion (en caso de estar a false estas ya en una)
    /// </summary>
    bool canStartConversation;

    /// <summary>
    /// tiempo minimo entre dialogos en milisegundos (no nos gustan los numeros magicos en el codigo a que no chicos?)
    /// </summary>
    Uint32 dialogueCooldownTime;

    /// <summary>
    /// timer para cooldown entre dos dialogos diferentes (no se podia hacer con el DelayedCallback component por ciertos cambios que se han 
    /// hecho para no estar anadiendo y quitando componentes con cada nuevo dialogo)
    /// </summary>
    Uint32 timer_;

    /// <summary>
    /// Tiempo al que el timer tiene que llegar (es el tiempo en el comenzo el timer + el cooldownTime)
    /// </summary>
    Uint32 dialogueCooldown;

    /// <summary>
    /// booleano que controla dicho timer
    /// </summary>
    bool controlTimer;

    /// <summary>
    /// entidad de la caja del dialogo
    /// </summary>
    ecs::Entity* boxBackground;

    /// <summary>
    /// entidad del texto del dialogo
    /// </summary>
    ecs::Entity* textDialogue;

    /// <summary>
    /// callback de final de dialogo
    /// </summary>
    std::function<void()> endDialogueCallback;
};