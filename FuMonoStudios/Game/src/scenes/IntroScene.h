#pragma once
#include <utils/checkML.h>
#include <architecture/Scene.h>
#include <components/Transform.h>
#include <components/Herramientas.h>
#include <sistemas/PaqueteBuilder.h>
#include <sistemas/DialogManager.h>

namespace ecs
{
    class IntroScene : public ecs::Scene
    {
        __SC_DECL__(ecs::sc::INTRO_SCENE)
    public:
        IntroScene();
        ~IntroScene();

        void update() override;

        void init() override;
    protected:
        /// <summary>
        /// metodo con toda la logica de la intro
        /// </summary>
        void updateIteration(int it);
        /// <summary>
        /// metodo que actualiza el dialogo
        /// </summary>
        void updateIntroDialogue();
        /// <summary>
        /// metodo para ejecutar callbacks con un delay 
        /// </summary>
        void delayedCallback(float time, SimpleCallback call);
        /// <summary>
        /// metodo que cambia el fondo
        /// </summary>
        void changeBackground(const std::string& background);
        /// <summary>
        /// metodo que crea la tuberia de basura
        /// </summary>
        ecs::Entity* createGarbage();
        /// <summary>
        /// metodo que crea los paquetes de la intro
        /// </summary>
        void createIntroPackage(Distrito dist, const std::string& remitente, TipoPaquete tp);
        /// <summary>
        /// metodo que crea la botella
        /// </summary>
        ecs::Entity* createBottle();

        ecs::Entity* createDoor();
        /// <summary>
        /// metodo que avanza la iteracion de la intro y actualiza todo
        /// </summary>
        void nextIteration();

        int introIteration; //entero que controla por que parte de la intro vamos

        bool waitingCallback; //flag para si tenemos un callback pendiente
        SimpleCallback call_; //callback a ejecutar
        Uint32 timer_; //control de tiempo de los delayed callbacks
        Uint32 timeToCall_;

        PaqueteBuilder* mPaqBuild_; 

        DialogManager mDialogManager;

        //punteros a entidades de la intro 
        ecs::Entity* fondo_; 
        ecs::Entity* tubo_;
        ecs::Entity* bottle_;
        ecs::Entity* carta_;
        ecs::Entity* jefe_;
        ecs::Entity* door_;
    };
}