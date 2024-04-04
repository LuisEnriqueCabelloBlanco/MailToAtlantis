#pragma once
#include "../architecture/Component.h"
#include <string>

/// <summary>
/// Struct de las pulls de sonidos. Tiene un string que representa el nombre base de todos los sonidos en la pull,
/// el numero de sonidos en esa pull y una comprobacion del ultimo sonido reproducido
/// </summary>
struct SoundPull {
	std::string baseFileName;
	int pullNumber;
	int lastPlayed;
};

/// <summary>
/// Componente que se ocupará de manejar el sonido dentro de entidades.
/// </summary>
class SoundEmiter : public ecs::Component
{
public:
	SoundEmiter();
	~SoundEmiter();

	//TODO: No estoy seguro de como se va a manejar el sonido globalmente así que por ahora lo dejo 
	void setAllVolumes(int volume);
	void setChannelVolume(int id, int volume);

	/// <summary>
	/// Añade el numbre del sonido indicado a la lista de sonidos 
	/// </summary>
	/// <param name="soundName">Nombre del archivo de sonido</param>
	/// <returns>Indice del sonido añadido</returns>
	int addSoundToLib(std::string soundName);

	void playSound(int id);
private:
	int volume_;
	
	/// <summary>
	/// Vector con los sonidos. Cada componente que requiera de sonidos le pasara en el init o la constructora el nombre del sonido
	/// (por ejemplo, 'arrastrar') y será devuelto un indice en el vector para cuando se quiera reproducir. 
	/// </summary>
	std::vector<SoundPull> soundLib_;
};

