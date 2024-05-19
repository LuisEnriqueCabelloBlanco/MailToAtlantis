#pragma once
#include <architecture/Component.h>
#include <functional>
#include <components/Paquete.h>
#include <architecture/Entity.h>

using Callback = std::function<void(ecs::Entity*)>;

enum TipoHerramienta {
	SelloCalleA, SelloCalleB, SelloCalleC, SelloMultiColor, SelloVacio, Polvos
};

// Para usar esta clase, hay que asignarla al entity y luego
// llamar a setFunctionality para saber qu� har� al interactuar con el
// paquete. 
// Funciona a la par con una funci�n que est� asignada a cada paquete
// que llama a la funcion interact si detecta que es una herramienta
class Herramientas : public ecs::Component
{
public:
	__CMP_DECL__(ecs::cmp::HERRAMIENTAS)
	Herramientas();

	~Herramientas();
	
	void initComponent() override {};

	void update() override {};
	/// <summary>
	/// Establece la funcionalidad de la herramienta
	/// </summary>
	/// <param name="tipo"></param>
	void setFunctionality(TipoHerramienta tipo);
	/// <summary>
	/// Interactua con otra entidad
	/// </summary>
	/// <param name="paquete"></param>
	void interact(ecs::Entity* paquete);
	/// <summary>
	/// Devuelve si es un sello multicolor
	/// </summary>
	/// <returns></returns>
	bool getMulticolorStamp() { return multicolorStamp; }
private:
	bool multicolorStamp;
	/// <summary>
	/// Funcion llamada al interacctuar con una entidad
	/// </summary>
	Callback funcion_;
};

