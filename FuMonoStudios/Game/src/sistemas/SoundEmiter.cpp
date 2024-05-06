#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "SoundEmiter.h"
#include <sdlutils/SDLUtils.h>
#include <json/JSON.h>
#include <exception>

const int MAX_CHANNELS = 3;


SoundEmiter::SoundEmiter() : soundVolume_(100), musicVolume_(100), playInChannel_(0)
{

}

SoundEmiter::~SoundEmiter()
{
	haltAllSounds();
}

void SoundEmiter::init()
{
	processSoundListJSON();
	SoundEffect::setNumberofChannels(MAX_CHANNELS);
}

void SoundEmiter::close()
{
	haltAllSounds();
	haltAllMusic();
}

void SoundEmiter::setSoundVolumes(int volume)
{
	std::cout << volume + "\n";
	soundVolume_ = volume;
	/*
	for (auto& it : soundPulls_) {
		for (int i = 0; i < it.second.first; i++) {
			sdlutils().soundEffects().at(it.first + std::to_string(i)).setVolume(soundVolume_);
		}
	}
	*/
}

void SoundEmiter::muteSingleSound(std::string sound, bool mute)
{
	try {
		auto it = soundPulls_.at(sound);
		it.mute = mute;
		for (int i = 0; i < it.amount; i++) {
			std::string fileName = sound + std::to_string(i);
			if (mute) {
				sdlutils().soundEffects().at(fileName).setVolume(0);
			}
			else {
				sdlutils().soundEffects().at(fileName).setVolume(soundVolume_);
			}
		}
	}
	catch (...) {
		throw std::exception("No existe ese sonido.");
	}
}

void SoundEmiter::playSound(std::string sound)
{
	try {
		auto it = soundPulls_.at(sound);
		if (!it.mute) {
			int am = it.amount;
			int rnd = sdlutils().rand().nextInt(0, am);
			std::string fileName = sound + std::to_string(rnd);
			std::cout << fileName << "\n";
			sdlutils().soundEffects().at(fileName).setVolume(soundVolume_);
			sdlutils().soundEffects().at(fileName).play(0, playInChannel_);
			it.lastChannel = playInChannel_;
			changeChannel();
		}
	}
	catch (...) {
		throw std::exception("No existe ese sonido.");
	}
}

void SoundEmiter::playSound(std::string sound, float modifier)
{
	try {
		auto it = soundPulls_.at(sound);
		if (!it.mute) {
			int am = it.amount;
			int rnd = sdlutils().rand().nextInt(0, am);
			std::string fileName = sound + std::to_string(rnd);
			std::cout << fileName << "\n";
			sdlutils().soundEffects().at(fileName).setVolume(soundVolume_ * modifier);
			sdlutils().soundEffects().at(fileName).play(0, playInChannel_);
			it.lastChannel = playInChannel_;
			changeChannel();
		}
	}
	catch (...) {
		throw std::exception("No existe ese sonido.");
	}
}

void SoundEmiter::haltSound(std::string sound)
{
	try {
		auto it = soundPulls_.at(sound);
		for (int i = 0; i < it.amount; i++) {
			sdlutils().soundEffects().at(sound + std::to_string(i)).haltChannel(it.lastChannel);
		}
	}
	catch (...) {
		throw std::exception("Ha ocurrido un error al intentar detener ese sonido.");
	}
}

void SoundEmiter::setMusicVolume(int volume)
{
	musicVolume_ = volume;
	for (auto i = activeSongs_.begin(); i != activeSongs_.end(); i++) {
		if ((*i).second) {
			sdlutils().musics().at((*i).first).setMusicVolume(musicVolume_);
		}
	}
}

void SoundEmiter::playMusic(std::string song)
{
	try {
		if (activeSongs_.find(song) != activeSongs_.end()) {
			sdlutils().musics().at(song).play();
			sdlutils().musics().at(song).setMusicVolume(musicVolume_);
		}
		else {
			activeSongs_.insert({ song, true });
			sdlutils().musics().at(song).play();
			sdlutils().musics().at(song).setMusicVolume(musicVolume_);
		}
	}
	catch (...) {
		throw std::exception("No existe esa m�sica.");
	}
}

void SoundEmiter::haltMusic(std::string song)
{
	try {
		sdlutils().musics().at(song).haltMusic();
		activeSongs_.at(song) = false;
	}
	catch (...) {
		throw std::exception("No existe esa m�sica.");
	}
}

void SoundEmiter::processSoundListJSON()
{
	std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile("recursos/config/sounds.json"));

	// check it was loaded correctly
	// the root must be a JSON object
	if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
		throw std::exception("Something went wrong while loading sound pulls");
	}

	JSONObject root = jValueRoot->AsObject();
	JSONValue* jValue = nullptr;

	jValue = root["sounds"];

			
	//recorrido de los sonidos
	for (auto& v : jValue->AsArray()) {
		if (v->IsObject()) {
			JSONObject vObj = v->AsObject();
			std::string key = vObj["key"]->AsString();
			int ammount = vObj["amount"]->AsNumber();
			soundPulls_.insert({ key, {ammount, false} });
		}
		else {
			throw std::exception("Something went wrong while loading sound");
		}
	}
}

void SoundEmiter::haltAllSounds()
{
	for (int i = -1; i < MAX_CHANNELS; i++) {
		sdlutils().soundEffects().at("click0").haltChannel(i); 
	}
}

void SoundEmiter::haltAllMusic()
{
	for (auto i = activeSongs_.begin(); i != activeSongs_.end(); i++) {
		if ((*i).second) {
			sdlutils().musics().at((*i).first).haltMusic();
		}
	}
}

void SoundEmiter::changeChannel()
{
	std::cout << playInChannel_ << " ";
	playInChannel_++;
	if (playInChannel_ == MAX_CHANNELS) {
		playInChannel_ = 0;
	}
	std::cout << playInChannel_ << "\n";
}
