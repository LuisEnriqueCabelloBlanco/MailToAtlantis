#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <architecture/Component.h>
#include <sistemas/DialogManager.h>
#include <sdlutils/Font.h>
/*
TODO:
Reestructurar toda la gestion de los dialogos
Crear las estructuras de datos para no tener que cargar los datos constantemente
Hacer un sistema de como relizar las solicitudes de un dialogo en funcion de las estructuras creadas
*/

class Transform;
class RenderImage;
class Font;
class DialogManager;
class Texture;
namespace ecs {
    class ExplorationScene;
    class TutorialScene;
}

/*
Componente que gestiona el renderizado del texto segun la iformacion que le pasa el dialogManager
renderiza el texto caracter a caracter y luego se espera a que se pulse el espacio para saltar el dialogo
Necesita de un Dialog manager
*/
class DialogComponent :
    public ecs::Component
{
public:
    __CMP_DECL__(ecs::cmp::DIALOGUE)

    //mas adelante hacer que la fuente se inicie solaz
    DialogComponent(DialogManager* manager);
    ~DialogComponent();
    void initComponent() override;
    void update() override;
private:
    /// <summary>
    /// Metodo para actualizar la textura de dialogo
    /// </summary>
    void setCurrentDialogue();

    Transform* mTr_;
    RenderImage* mRend_;
    DialogManager* mDialogMngr_;

    /// <summary>
    /// Fuente del texto de los dialogos
    /// </summary>
    Font* mFont_;
    /// <summary>
    /// Textura de dialogo
    /// </summary>
    Texture* mTexture_;
    /// <summary>
    /// Ancho maximo al que va a llegar el texto
    /// </summary>
    int dialogueWidth_;
    /// <summary>
    /// Indice de hasta que caracter se va a escribir
    /// </summary>
    int dialogueIndex_;

    /// <summary>
    /// flag para saber cuando hay que cerrar la caja de dialogo
    /// </summary>
    bool endDialogue;

    /// <summary>
    /// flag para saber si puedes pulsar skipear o no
    /// </summary>
    bool canSkip;

    uint32_t lastTimePaused_ = 0;
};

