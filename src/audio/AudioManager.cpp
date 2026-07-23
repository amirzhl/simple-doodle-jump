// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "audio/AudioManager.hpp"

#include "core/Constants.hpp"

#include <exception>

int AudioManager::cfg_default_volume() {
	return cfg::audio::DEFAULT_VOLUME;
}

AudioManager::AudioManager() {
	// Pre-create a fixed pool of effect voices so playback never allocates.
	voices_.resize(static_cast<std::size_t>(cfg::audio::SOUND_POOL_SIZE));
	setVolume(volume_);
}

void AudioManager::setVolume(int volume) {
	volume_ = volume;
	if (volume_ < cfg::audio::MIN_VOLUME) {
		volume_ = cfg::audio::MIN_VOLUME;
	}
	if (volume_ > cfg::audio::MAX_VOLUME) {
		volume_ = cfg::audio::MAX_VOLUME;
	}
	const float sfml_volume = static_cast<float>(volume_);
	music_.setVolume(sfml_volume);
	for (sf::Sound& voice : voices_) {
		voice.setVolume(sfml_volume);
	}
}

void AudioManager::playMenuMusic() {
	if (!music_ready_) {
		if (!music_.openFromFile(cfg::sound::MENU_MUSIC)) {
			return; // missing music must not crash the game
		}
		music_.setLoop(true);
		music_.setVolume(static_cast<float>(volume_));
		music_ready_ = true;
	}
	if (music_.getStatus() != sf::SoundSource::Playing) {
		music_.play();
	}
}

void AudioManager::stopMusic() {
	music_.stop();
}

void AudioManager::playJump() {
	playEffect(cfg::sound::SFX_JUMP);
}

void AudioManager::playShoot() {
	playEffect(cfg::sound::SFX_SHOOT);
}

void AudioManager::playLose() {
	playEffect(cfg::sound::SFX_LOSE);
}

void AudioManager::playEffect(const std::string& path) {
	if (voices_.empty()) {
		return;
	}
	try {
		sf::SoundBuffer& buffer = buffers_.acquire(path); // cached, loaded once
		sf::Sound& voice = voices_[next_voice_];
		next_voice_ = (next_voice_ + 1) % voices_.size();
		voice.setBuffer(buffer);
		voice.setVolume(static_cast<float>(volume_));
		voice.play();
	} catch (const std::exception&) {
		// A missing or corrupt sound file must never crash the gameplay.
	}
}
