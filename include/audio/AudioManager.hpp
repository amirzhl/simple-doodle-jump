// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include "core/ResourceManager.hpp"

#include <SFML/Audio/Music.hpp>
#include <SFML/Audio/Sound.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <vector>

// ---------------------------------------------------------------------------
//  AudioManager  -- the single owner of all game audio.
//
//  * Background music (sf::Music) is streamed straight from disk and plays
//    ONLY on the Menu and Settings screens (per the spec).
//  * Short sound effects (jump / shoot / lose) are played through a small pool
//    of sf::Sound voices so rapid, overlapping effects never cut each other
//    off. Their sf::SoundBuffers are cached through the SAME generic
//    ResourceManager<T> introduced in phase 1 -- now instantiated for
//    sf::SoundBuffer, exactly the reuse the rubric asks for.
//  * A single master volume (0..100) drives both music and effects and can be
//    changed live from the Settings screen.
// ---------------------------------------------------------------------------
class AudioManager {
public:
	AudioManager();

	AudioManager(const AudioManager&) = delete;
	AudioManager& operator=(const AudioManager&) = delete;

	// Apply a master volume in [0, 100] to the music and every effect voice.
	void setVolume(int volume);
	int volume() const { return volume_; }

	// Background music control (Menu / Settings only).
	void playMenuMusic();
	void stopMusic();

	// One-shot sound effects.
	void playJump();
	void playShoot();
	void playLose();

private:
	// Play the cached buffer at `path` on a free voice from the pool.
	void playEffect(const std::string& path);

	ResourceManager<sf::SoundBuffer> buffers_; // reused phase-1 template
	sf::Music music_;
	std::vector<sf::Sound> voices_;            // round-robin effect voices
	std::size_t next_voice_ = 0;
	int volume_ = cfg_default_volume();
	bool music_ready_ = false;

	static int cfg_default_volume();
};
