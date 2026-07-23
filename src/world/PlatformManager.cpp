// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "world/PlatformManager.hpp"

#include "core/Constants.hpp"
#include "core/ResourceManager.hpp"
#include "entities/platforms/BreakablePlatform.hpp"
#include "entities/platforms/MovingPlatform.hpp"
#include "entities/platforms/NormalPlatform.hpp"
#include "entities/platforms/Platform.hpp"
#include "utils/Random.hpp"

#include <SFML/Graphics/Texture.hpp>

PlatformManager::PlatformManager(ResourceManager<sf::Texture>& textures,
                                 float moving_speed)
	: textures_(textures), moving_speed_(moving_speed) {}

PlatformManager::~PlatformManager() {
	clear();
}

void PlatformManager::clear() {
	for (Platform* platform : platforms_) {
		delete platform; // every platform is heap-allocated -> free it here
	}
	platforms_.clear();
}

void PlatformManager::reset() {
	clear();

	const float screen_width = static_cast<float>(cfg::window::WIDTH);
	const float screen_height = static_cast<float>(cfg::window::HEIGHT);

	// A guaranteed, spring-free normal platform right under the player start.
	const sf::Texture& normal_tex = textures_.acquire(cfg::assets::NORMAL_PLATFORM);
	const float start_x = screen_width / 2.0f - normal_tex.getSize().x / 2.0f;
	const float start_y = screen_height - 90.0f;
	auto* start = new NormalPlatform(normal_tex, sf::Vector2f(start_x, start_y));
	platforms_.push_back(start);
	starting_platform_top_ = start->getBounds().top;
	highest_y_ = start_y;

	// Fill the rest of the initial screen upward.
	generateAbove(0.0f - cfg::world::GENERATION_BUFFER);
}

void PlatformManager::update(float dt, float world_top, float world_bottom) {
	for (Platform* platform : platforms_) {
		platform->update(dt);
	}
	removeExpired(world_bottom);
	generateAbove(world_top);
}

bool PlatformManager::lastPlatformIsBreakable() const {
	if (platforms_.empty()) {
		return false;
	}
	// Safe, run-time type identification of the concrete platform kind. If the
	// last platform really is a BreakablePlatform the cast yields a valid
	// pointer; otherwise it yields nullptr. This is the spec's suggested use of
	// dynamic_cast for detecting the platform type.
	return dynamic_cast<const BreakablePlatform*>(platforms_.back()) != nullptr;
}

void PlatformManager::generateAbove(float world_top) {
	// Keep producing platforms until we have covered the buffer above the view.
	while (highest_y_ > world_top - cfg::world::GENERATION_BUFFER) {
		const float gap = Random::floatInRange(cfg::world::MIN_GAP,
		                                       cfg::world::MAX_GAP);
		const float y = highest_y_ - gap;

		// A platform's width is needed to keep x inside the screen. Use the
		// normal platform width as the reference (all are similar).
		const sf::Texture& normal_tex =
			textures_.acquire(cfg::assets::NORMAL_PLATFORM);
		const float width = static_cast<float>(normal_tex.getSize().x);
		const float max_x = static_cast<float>(cfg::window::WIDTH) - width;
		const float x = Random::floatInRange(0.0f, max_x);

		// Never allow two breakable platforms in a row (accessibility). The type
		// of the previous platform is detected with dynamic_cast.
		const bool allow_breakable = !lastPlatformIsBreakable();
		Platform* platform = createPlatform(x, y, allow_breakable);
		platforms_.push_back(platform);
		highest_y_ = y;
	}
}

Platform* PlatformManager::createPlatform(float x, float y, bool allow_breakable) {
	const sf::Vector2f pos(x, y);

	const int breakable_weight =
		allow_breakable ? cfg::platform::WEIGHT_BREAKABLE : 0;
	const int choice = Random::weightedChoice(cfg::platform::WEIGHT_NORMAL,
	                                          cfg::platform::WEIGHT_MOVING,
	                                          breakable_weight);

	if (choice == 1) {
		return new MovingPlatform(textures_.acquire(cfg::assets::MOVING_PLATFORM),
		                          pos, moving_speed_);
	}
	if (choice == 2) {
		return new BreakablePlatform(
			textures_.acquire(cfg::assets::BROKEN_PLATFORM), pos);
	}

	// Normal platform -- may randomly carry a spring.
	auto* normal =
		new NormalPlatform(textures_.acquire(cfg::assets::NORMAL_PLATFORM), pos);
	if (Random::chance(cfg::platform::SPRING_CHANCE)) {
		normal->attachSpring(textures_.acquire(cfg::assets::SPRING));
	}
	return normal;
}

void PlatformManager::removeExpired(float world_bottom) {
	std::vector<Platform*> survivors;
	survivors.reserve(platforms_.size());
	for (Platform* platform : platforms_) {
		if (platform->isExpired(world_bottom)) {
			delete platform; // scrolled off the bottom -> free it (no leaks)
		} else {
			survivors.push_back(platform);
		}
	}
	platforms_.swap(survivors);
}
