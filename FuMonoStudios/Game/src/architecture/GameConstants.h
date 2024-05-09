#pragma once
#include <string>

#pragma region Paths
/*========================================================================================
* PATHS
*=====================================================================================*/
const std::string DIFF_SETTINGS_PATH = "recursos/config/mail.dificulty.json";
const std::string REMITENT_SETTINGS_PATH = "recursos/config/mail.destinatarios.json";
const std::string DIR_SETTINGS_PATH = "recursos/config/mail.direcctions.json";
#pragma endregion

#pragma region General
/*========================================================================================
* GENERAL CONSTANTS
*=====================================================================================*/
constexpr int INITIAL_MONEY = 10000;
constexpr int INITIAL_FINAL = 0;
constexpr int INITIAL_EVENT = 0;
constexpr int INITIAL_DAY = 1;
constexpr int INITIAL_TUBE_AMOUNT = 1;
constexpr int WRITE_PACAGES_VALUE = 20;
constexpr int WRONG_PACAGES_VALUE = 30;
constexpr int MINIGAME_TIME = 1;
constexpr int INITIAL_FAILS_MARGIN = 1;
constexpr int LOGICAL_RENDER_WIDTH = 1920;
constexpr int LOGICAL_RENDER_HEITH = 1080;
constexpr int MAX_DISTRICTS = 7;
#pragma endregion


#pragma region Package
/*========================================================================================
* PACKAGE SETTINGS
*=====================================================================================*/
constexpr int PESADO_MAX = 75;	//L�mite del peso m�ximo de paquetes pesados 
constexpr int MEDIO_MAX = 50;	//L�mite del peso m�ximo de paquetes de peso medio 
constexpr int LIGERO_MAX = 25;	//L�mite del peso m�ximo de paquetes ligeros
constexpr int PAQUETE_MIN = 10;	//L�mite del peso m�nimo de paquetes ligeros
constexpr int PESO_CARTA = 2;	//Peso carta
// posicion y tamano Tipo sellos
constexpr int TIPO_SELLO_POS_X = 10;
constexpr int TIPO_SELLO_POS_Y = 60;
constexpr int TIPO_SELLO_SIZE = 80;
// posicion y tamano Fragil sellos
constexpr int FRAGIL_SELLO_POS_X = 100;
constexpr int FRAGIL_SELLO_POS_Y = 60;
constexpr int FRAGIL_SELLO_SIZE = 80;
// posicion y tamano Peso sellos
constexpr int PESO_SELLO_POS_X = 190;
constexpr int PESO_SELLO_POS_Y = 60;
constexpr int PESO_SELLO_SIZE = 80;
//Escala del paquete 
constexpr float PAQUETE_SIZE = 0.25f;
#pragma endregion
