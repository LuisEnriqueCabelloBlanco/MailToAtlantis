#pragma once
#include <string>
#include <unordered_map>
#include "../utils/Singleton.h"

/// <summary>
/// Componente que se ocupará de manejar el sonido dentro de entidades.
/// </summary>
class SoundEmiter : public Singleton<SoundEmiter>
{
public:
	friend Singleton<SoundEmiter>;

	SoundEmiter();
	~SoundEmiter();

	void init();

	void setAllVolumes(int volume);

	void muteSingleSound(std::string sound, bool mute);

	void playSound(std::string sound);
	void haltSound(std::string sound);
	void playMusic(std::string song);
private:
	void processSoundListJSON();

	int volume_;
	
	/// <summary>
	/// Mapa de los soundPulls. La clave es el nombre del sonido y el int es cuántos sonidos hay en esa pull.
	/// </summary>
	std::unordered_map<std::string, int> soundPulls_;
};

