#pragma once
#include <utils/checkML.h>
#include <string>
#include <unordered_map>
#include "../utils/Singleton.h"

struct SoundInfo {
	int amount;
	bool mute;
	int lastChannel;
};
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
	void close();

	void setSoundVolumes(int volume);
	void muteSingleSound(std::string sound, bool mute);

	void playSound(std::string sound);
	void playSound(std::string sound, float modifier);
	void haltSound(std::string sound);

	void setMusicVolume(int volume);

	void playMusic(std::string song);
	void haltMusic(std::string song);
private:
	void processSoundListJSON();

	void haltAllSounds();
	void haltAllMusic();

	int soundVolume_;
	int musicVolume_;
	
	/// <summary>
	/// Mapa de los soundPulls. La clave es el nombre del sonido y el int es cuántos sonidos hay en esa pull.
	/// </summary>
	std::unordered_map<std::string, SoundInfo> soundPulls_;

	/// <summary>
	/// Mapa de las canciones
	/// </summary>
	std::unordered_map<std::string, bool> activeSongs_;

	//Manejo de canales

	int playInChannel_;

	void changeChannel();
};


inline SoundEmiter& soundEmiter() {
	return *SoundEmiter::instance();
}