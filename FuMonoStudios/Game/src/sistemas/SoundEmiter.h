#pragma once
#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include <string>
#include <unordered_map>
#include <utils/Singleton.h>

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

	//Llamada al primero usar SoundEmitter
	void init();
	//Llamada al cerrar cada escena
	void close();

	//Modifica el sonido de los SFX
	void setSoundVolumes(int volume);
	//Mutea un sonido
	void muteSingleSound(const std::string& sound, bool mute);

	//Activa un SFX
	void playSound(const std::string& sound, int loops = 0);
	//Activa un SFX a cierto volumen
	/// <param name="modifier"> entre 0 y 1 </param>
	void playSound(const std::string& sound, float modifier, int loops = 0);
	//Detiene un SFX
	void haltSound(const std::string& sound);

	//Modifica el sonido de la música
	void setMusicVolume(int volume);

	//Activa una canción
	void playMusic(const std::string& song);
	//Detiene una canción
	void haltMusic(const std::string& song);
private:
	//Procesa el Json de sounds.json
	void processSoundListJSON();

	//Detiene todos los SFX
	void haltAllSounds();
	//Detiene toda las canciones
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