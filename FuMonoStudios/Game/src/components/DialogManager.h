// dialog_manager.h
#pragma once

#include <string>
#include <vector>
#include <iostream>

class GeneralData;

/*
Clase que genera los dialogos que se van a escribir
Carga los dialogos de un fichero te texto que tenemos codificado
*/
class DialogManager {
    friend GeneralData;
public:
    DialogManager();

    /// <summary>
    /// enum con todos los tipos de dialogos
    /// Los tipos de Presentaci�n, felicidad, genericos y eventos est�n dedicados a NPCmenores,
    /// mientras que los de NPCgrande para los grandes. El NOTYPE se usa para cosas interactuables
    /// del entorno que usen solo un di�logo.
    /// </summary>
    enum TipoDialog { Presentacion, FelMinimo, FelMaximo, 
        GenericosMalo, GenericosNormal, GenericosBueno, 
        Eventos, NPCgrande, NPCgrandePostConversacion, NOTYPE };

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
    
    // Establece los dialogos del personaje introducido, y busca dentro de su
    // seccion en el json el tipoDialogo, mas el numero introducido en el 
    // dialogueSelection. Poner un -1 para no usar el numero de dialogueSelection
    void setDialogues(const GeneralData::Personaje pers, const TipoDialog tipoDialogo, int dialogueSelection);
    void setDialogues(const GeneralData::Personaje a, const TipoDialog t) { setDialogues(a, t, -1); }

    // convierte TipoDialog a string y a�ade el indice de dialogo en caso de usarlo
    const std::string tipoDialogToString(const TipoDialog tipo, int dialogueSelection);
private:
    /// <summary>
    /// Vector donde se almacenan todos los di�logos que se van a soltar
    /// </summary>
    std::vector<std::string> dialogs_;
    /// <summary>
    /// Indice que indica en que di�logo nos encontramos
    /// </summary>
    size_t currentDialogIndex_;
};