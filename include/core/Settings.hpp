// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include "core/Difficulty.hpp"

#include <string>

// ---------------------------------------------------------------------------
//  Settings  -- the persistent user preferences edited from the Settings
//  screen: the master volume (0..100) and the selected difficulty. Loaded
//  from and saved to `settings.txt` so choices survive between runs.
// ---------------------------------------------------------------------------
class Settings {
public:
	// Read preferences from `path`. Missing/invalid files keep the defaults.
	void load(const std::string& path);
	// Persist the current preferences to `path`.
	void save(const std::string& path) const;

	int volume() const { return volume_; }
	void setVolume(int volume); // clamped to [MIN_VOLUME, MAX_VOLUME]

	Difficulty difficulty() const { return difficulty_; }
	void setDifficulty(Difficulty level) { difficulty_ = level; }

private:
	int volume_ = cfg_default_volume();
	Difficulty difficulty_ = Difficulty::Medium;

	// Small helper so the default lives in Constants without a header cycle in
	// the class body; defined in the .cpp.
	static int cfg_default_volume();
};
