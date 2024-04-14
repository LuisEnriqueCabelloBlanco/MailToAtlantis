// dialog_manager.h
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "../architecture/GeneralData.h"

/*
Clase que genera los dialogos que se van a escribir
Carga los dialogos del json dialogos.json

Para cargar una entrada en el json, debemos usar el enum DialogSelection.
Este sistema irá añadiendo los strings al dialogComponent.
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
        JefeOficina, Tutorial, BryantMyers
    };

    DialogManager();

    /// <summary>
    /// Devuelve el dialogo acutal segun el indice de dialogo
    /// </summary>
    /// <returns></returns>
    std::string getCurrentDialog();

    /// <summary>
    /// Avanza al siguente dialogo. 
    /// Si ya está en el ultimo diálogo devuelve false y resetea
    /// el index a 0
    /// </summary>
    bool nextDialog();
    
    // Establece los dialogos del tipo introducido mirar arriba para saber cual usar
    void setDialogues(const DialogSelection ds, const std::string& tipoDialogo, int dialogueSelection);
    void setDialogues(const DialogSelection ds, const std::string& t) { setDialogues(ds, t, -1); }
    void setDialogues(const DialogSelection ds) { setDialogues(ds, "NULL", -1); }
private:
    void fixText(std::string& text);
    void crearTildes(std::string& aux);

    std::string dialogSelectionToString(const DialogSelection ds);

    bool isNPC(const DialogSelection ds);
    /// <summary>
    /// Vector donde se almacenan todos los diálogos que se van a soltar
    /// </summary>
    std::vector<std::string> dialogs_;
    /// <summary>
    /// Indice que indica en que diálogo nos encontramos
    /// </summary>
    size_t currentDialogIndex_;
};