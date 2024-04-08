#include "SoundEmiter.h"
#include "../sdlutils/SDLUtils.h"
#include "../json/JSON.h"

SoundEmiter::SoundEmiter() : volume_(100)
{

}

SoundEmiter::~SoundEmiter()
{

}

void SoundEmiter::init()
{
	processSoundListJSON();
}

void SoundEmiter::setAllVolumes(int volume)
{
	volume_ = volume;
}

void SoundEmiter::muteSingleSound(std::string sound, bool mute)
{
	int it = soundPulls_.at(sound);
	if (mute) {
		for (int i = 0; i < it; i++) {
			sdlutils().soundEffects().at(sound + std::to_string(i)).setVolume(0);
		}
	}
	else {
		for (int i = 0; i < it; i++) {
			sdlutils().soundEffects().at(sound + std::to_string(i)).setVolume(volume_);
		}
	}
}

void SoundEmiter::playSound(std::string sound)
{
	int it = soundPulls_.at(sound);
	int rnd = sdlutils().rand().nextInt(0, it);
	std::string fileName = sound + std::to_string(rnd);
	std::cout << fileName;
	sdlutils().soundEffects().at(fileName).play();
}

void SoundEmiter::haltSound(std::string sound)
{
	int it = soundPulls_.at(sound);
	for (int i = 0; i < it; i++) {
		sdlutils().soundEffects().at(sound + std::to_string(i)).haltChannel();
	}
}

void SoundEmiter::playMusic(std::string song)
{

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
			soundPulls_.insert({ key, ammount });
		}
		else {

			throw "Error uwu";

		}
	}
}
