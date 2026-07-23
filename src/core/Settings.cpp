// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "core/Settings.hpp"

#include "core/Constants.hpp"

#include <algorithm>
#include <fstream>

int Settings::cfg_default_volume() {
	return cfg::audio::DEFAULT_VOLUME;
}

void Settings::setVolume(int volume) {
	volume_ = std::max(cfg::audio::MIN_VOLUME,
	                   std::min(cfg::audio::MAX_VOLUME, volume));
}

void Settings::load(const std::string& path) {
	std::ifstream in(path);
	if (!in) {
		return; // keep defaults on first run
	}
	int volume = volume_;
	int difficulty_index = difficulty::toIndex(difficulty_);
	if (in >> volume) {
		setVolume(volume);
	}
	if (in >> difficulty_index) {
		difficulty_ = difficulty::fromIndex(difficulty_index);
	}
}

void Settings::save(const std::string& path) const {
	std::ofstream out(path, std::ios::trunc);
	if (!out) {
		return;
	}
	out << volume_ << '\n' << difficulty::toIndex(difficulty_) << '\n';
}
