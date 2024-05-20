#ifndef DEV_TOOLS
#include <utils/checkML.h>
#endif // !DEV_TOOLS
#include "SoundEmiter.h"
#include <sdlutils/SDLUtils.h>
#include <json/JSON.h>
#include <architecture/GameConstants.h>
#include <architecture/Exceptions.h>

const int MAX_CHANNELS = 3;


SoundEmiter::SoundEmiter() : soundVolume_(100), musicVolume_(100), playInChannel_(0), priorityChannel_(-1)
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
#ifdef _DEBUG
	std::cout << volume + "\n";
#endif // _DEBUG
	soundVolume_ = volume;
	/*
	for (auto& it : soundPulls_) {
		for (int i = 0; i < it.second.first; i++) {
			sdlutils().soundEffects().at(it.first + std::to_string(i)).setVolume(soundVolume_);
		}
	}
	*/
}

void SoundEmiter::muteSingleSound(const std::string& sound, bool mute)
{
	try {
		auto& it = soundPulls_.at(sound);
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
	catch (const std::string& e) {
		std::cerr << e << "\n" << std::endl;
	}
}

void SoundEmiter::playSound(const std::string& sound, int loops)
{
	playSound(sound, 1.0f, loops);
}

void SoundEmiter::playSound(const std::string& sound, float modifier, int loops)
{
	try {
		auto& it = soundPulls_.at(sound);
		if (!it.mute) {
			int am = it.amount;
			int rnd = sdlutils().rand().nextInt(0, am);
			std::string fileName = sound + std::to_string(rnd);
#ifdef _DEBUG
			std::cout << "Playing sound: " << fileName << "\n";
#endif // _DEBUG
			sdlutils().soundEffects().at(fileName).setVolume(soundVolume_ * modifier);
			sdlutils().soundEffects().at(fileName).play(loops, playInChannel_);
			it.lastChannel = playInChannel_;
			changeChannel();
		}
	}
	catch (const std::string& e) {
		std::cerr << e << "\n" << std::endl;
	}
}

void SoundEmiter::playSoundWithPriority(const std::string& sound, int loops)
{
	if(priorityChannel_ == -1) {
		priorityChannel_ = playInChannel_;
	}
	playSound(sound, 1, loops);
}

void SoundEmiter::haltSound(const std::string& sound)
{
	try {
		auto& it = soundPulls_.at(sound);
		for (int i = 0; i < it.amount; i++) {
			sdlutils().soundEffects().at(sound + std::to_string(i)).haltChannel(it.lastChannel);
		}
		if (it.lastChannel == priorityChannel_) {
			priorityChannel_ = -1;
		}
	}
	catch (const std::string& e) {
		std::cerr << e << "\n" << std::endl;
	}
}

void SoundEmiter::setMusicVolume(int volume)
{
	musicVolume_ = volume;
	for (auto i : activeSongs_) {
		if (i.second) {
			sdlutils().musics().at(i.first).setMusicVolume(musicVolume_ * 0.5);
		}
	}
}

void SoundEmiter::playMusic(const std::string& song)
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
	catch (const std::string& e) {
		std::cerr << e << "\n" << std::endl;
	}
}

void SoundEmiter::haltMusic(const std::string& song)
{
	try {
		sdlutils().musics().at(song).haltMusic();
		activeSongs_.at(song) = false;
	}
	catch (const std::string& e) {
		std::cerr << e << "\n" << std::endl;
	}
}

void SoundEmiter::processSoundListJSON()
{
	std::unique_ptr<JSONValue> jValueRoot(JSON::ParseFromFile(SOUNDS_PATH));

	// check it was loaded correctly
	// the root must be a JSON object
	if (jValueRoot == nullptr || !jValueRoot->IsObject()) {
		throw config_File_Missing(SOUNDS_PATH);
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
#ifdef _DEBUG
			std::cerr<< wrong_JSON_Format(SOUNDS_PATH).what() << std::endl;
#endif // _DEBUG

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
	for (auto i : activeSongs_) {
		if (i.second) {
			sdlutils().musics().at(i.first).haltMusic();
		}
	}
}

void SoundEmiter::changeChannel()
{
	playInChannel_ = (playInChannel_ + 1) % MAX_CHANNELS;
	if (playInChannel_ == priorityChannel_) {
		playInChannel_ = (playInChannel_ + 1) % MAX_CHANNELS;
	}
}
