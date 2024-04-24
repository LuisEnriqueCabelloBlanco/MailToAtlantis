#include "SoundEmiter.h"
#include "../sdlutils/SDLUtils.h"
#include "../json/JSON.h"
#include <exception>

SoundEmiter::SoundEmiter() : soundVolume_(100), musicVolume_(100)
{

}

SoundEmiter::~SoundEmiter()
{

}

void SoundEmiter::init()
{
	processSoundListJSON();
}

void SoundEmiter::setSoundVolumes(int volume)
{
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
		it.second = mute;
		for (int i = 0; i < it.first; i++) {
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
		throw "No existe ese sonido.";
	}
}

void SoundEmiter::playSound(std::string sound)
{
	try {
		auto it = soundPulls_.at(sound);
		if (!it.second) {
			int am = it.first;
			int rnd = sdlutils().rand().nextInt(0, am);
			std::string fileName = sound + std::to_string(rnd);
			std::cout << fileName << "\n";
			sdlutils().soundEffects().at(fileName).setVolume(soundVolume_);
			sdlutils().soundEffects().at(fileName).play();
		}
	}
	catch (...) {
		throw "No existe ese sonido.";
	}
}

void SoundEmiter::playSound(std::string sound, float modifier)
{
	try {
		auto it = soundPulls_.at(sound);
		if (!it.second) {
			int am = it.first;
			int rnd = sdlutils().rand().nextInt(0, am);
			std::string fileName = sound + std::to_string(rnd);
			std::cout << fileName << "\n";
			sdlutils().soundEffects().at(fileName).setVolume(soundVolume_ * modifier);
			sdlutils().soundEffects().at(fileName).play();
		}
	}
	catch (...) {
		throw "No existe ese sonido.";
	}
}

void SoundEmiter::haltSound(std::string sound)
{
	try {
		int it = soundPulls_.at(sound).first;
		for (int i = 0; i < it; i++) {
			sdlutils().soundEffects().at(sound + std::to_string(i)).haltChannel();
		}
	}
	catch (...) {
		throw "No existe ese sonido.";
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
		throw "No existe esa música.";
	}
}

void SoundEmiter::haltMusic(std::string song)
{
	try {
		sdlutils().musics().at(song).haltMusic();
		activeSongs_.at(song) = false;
	}
	catch (...) {
		throw "No existe esa música.";
	}
}

void SoundEmiter::processSoundListJSON()
{
	std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile("recursos/config/sounds.json"));

	// check it was loaded correctly
	// the root must be a JSON object
	if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
		throw "Something went wrong while loading sound pulls";
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
			throw "Error uwu";
		}
	}
}
