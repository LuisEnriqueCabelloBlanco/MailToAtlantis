#include "SoundEmiter.h"
#include "../sdlutils/SDLUtils.h"

SoundEmiter::SoundEmiter() : volume_(100)
{

}

SoundEmiter::~SoundEmiter()
{

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
	//sdlutils().soundEffects().at(filename).play();
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

}
