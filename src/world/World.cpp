// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "world/World.hpp"

#include "core/Constants.hpp"
#include "core/ResourceManager.hpp"
#include "entities/Hole.hpp"
#include "entities/Monster.hpp"
#include "entities/platforms/NormalPlatform.hpp"
#include "entities/platforms/Platform.hpp"
#include "utils/Random.hpp"

#include <SFML/Graphics/Texture.hpp>

float World::movingSpeedFor(Difficulty difficulty) {
	const float multiplier = (difficulty == Difficulty::Easy)
	                             ? cfg::hazard::MOVING_SPEED_MULT_EASY
	                             : cfg::hazard::MOVING_SPEED_MULT_HARD;
	return cfg::platform::MOVING_SPEED * multiplier;
}

World::World(ResourceManager<sf::Texture>& textures, Difficulty difficulty)
	: textures_(textures), difficulty_(difficulty),
	  platform_manager_(textures, movingSpeedFor(difficulty)) {}

World::~World() {
	for (Monster* monster : monsters_) {
		delete monster;
	}
	for (Hole* hole : holes_) {
		delete hole;
	}
}

int World::monsterHealth() const {
	switch (difficulty_) {
		case Difficulty::Easy:
			return cfg::monster::HEALTH_EASY;
		case Difficulty::Medium:
			return cfg::monster::HEALTH_MEDIUM;
		case Difficulty::Hard:
			return cfg::monster::HEALTH_HARD;
	}
	return cfg::monster::HEALTH_MEDIUM;
}

void World::reset() {
	for (Monster* monster : monsters_) {
		delete monster;
	}
	monsters_.clear();
	for (Hole* hole : holes_) {
		delete hole;
	}
	holes_.clear();

	platform_manager_.reset();
	elapsed_ = 0.0f;
	spawn_timer_ = cfg::hazard::MIN_SPAWN_INTERVAL;
}

void World::update(float dt, float world_top, float world_bottom) {
	elapsed_ += dt;
	platform_manager_.update(dt, world_top, world_bottom);
	updateMonsters(dt, world_bottom);
	updateHoles(world_bottom);

	// No hazards at the very start of a run; afterwards spawn on a randomised
	// cadence so they appear with a limited probability.
	if (elapsed_ >= cfg::hazard::START_DELAY) {
		spawn_timer_ -= dt;
		if (spawn_timer_ <= 0.0f) {
			spawn_timer_ = Random::floatInRange(cfg::hazard::MIN_SPAWN_INTERVAL,
			                                    cfg::hazard::MAX_SPAWN_INTERVAL);
			maybeSpawnHazard();
		}
	}
}

void World::updateMonsters(float dt, float world_bottom) {
	std::vector<Monster*> survivors;
	survivors.reserve(monsters_.size());
	for (Monster* monster : monsters_) {
		monster->update(dt);
		if (!monster->isAlive() || monster->isExpired(world_bottom)) {
			delete monster; // killed or scrolled away -> free it (no leaks)
		} else {
			survivors.push_back(monster);
		}
	}
	monsters_.swap(survivors);
}

void World::updateHoles(float world_bottom) {
	std::vector<Hole*> survivors;
	survivors.reserve(holes_.size());
	for (Hole* hole : holes_) {
		if (hole->isExpired(world_bottom)) {
			delete hole;
		} else {
			survivors.push_back(hole);
		}
	}
	holes_.swap(survivors);
}

bool World::areaIsFree(const sf::FloatRect& candidate) const {
	const float pad = cfg::hazard::OVERLAP_PADDING;
	const sf::FloatRect padded(candidate.left - pad, candidate.top - pad,
	                           candidate.width + 2.0f * pad,
	                           candidate.height + 2.0f * pad);

	for (const Platform* platform : platform_manager_.platforms()) {
		if (padded.intersects(platform->getBounds())) {
			return false;
		}
		// Springs stick out above their platform, so their footprint counts
		// too (dynamic_cast: only a NormalPlatform can carry a spring).
		const auto* normal = dynamic_cast<const NormalPlatform*>(platform);
		if (normal && normal->hasSpring() &&
		    padded.intersects(normal->spring()->getBounds())) {
			return false;
		}
	}
	for (const Monster* monster : monsters_) {
		if (padded.intersects(monster->getBounds())) {
			return false;
		}
	}
	for (const Hole* hole : holes_) {
		if (padded.intersects(hole->getBounds())) {
			return false;
		}
	}
	return true;
}

void World::maybeSpawnHazard() {
	std::vector<Platform*>& platforms = platform_manager_.platforms();
	if (platforms.empty()) {
		return;
	}

	// Anchor the hazard beside the most recently generated (topmost) platform.
	const sf::FloatRect anchor = platforms.back()->getBounds();
	const float screen_width = static_cast<float>(cfg::window::WIDTH);

	// Decide the hazard kind. Holes exist on Hard only.
	const bool make_hole = difficulty::holesEnabled(difficulty_) &&
	                       Random::chance(cfg::hazard::HOLE_SHARE);

	// Pick the texture (random variant / size) and its footprint.
	const sf::Texture* texture = nullptr;
	float scale = 1.0f;
	if (make_hole) {
		texture = &textures_.acquire(Random::chance(0.5f) ? cfg::assets::HOLE_SMALL
		                                                  : cfg::assets::HOLE_LARGE);
	} else {
		texture = &textures_.acquire(Random::chance(0.5f)
		                                 ? cfg::assets::BLUE_MONSTER
		                                 : cfg::assets::GREEN_MONSTER);
		scale = cfg::monster::SCALE;
	}
	const float hazard_w = static_cast<float>(texture->getSize().x) * scale;
	const float hazard_h = static_cast<float>(texture->getSize().y) * scale;

	// Place it on whichever side of the platform has more room.
	const float anchor_center = anchor.left + anchor.width / 2.0f;
	float x = (anchor_center < screen_width / 2.0f)
	              ? anchor.left + anchor.width + cfg::hazard::SIDE_GAP
	              : anchor.left - cfg::hazard::SIDE_GAP - hazard_w;
	if (x < 0.0f) {
		x = 0.0f;
	}
	if (x + hazard_w > screen_width) {
		x = screen_width - hazard_w;
	}
	const float y = anchor.top - hazard_h; // rest just above the platform row

	const sf::FloatRect candidate(x, y, hazard_w, hazard_h);
	if (!areaIsFree(candidate)) {
		return; // keep everything non-overlapping (spec 7.1)
	}

	if (make_hole) {
		holes_.push_back(new Hole(*texture, sf::Vector2f(x, y)));
	} else {
		// Restrict the patrol to a corridor around the spawn point (spec 2.2:
		// a monster must never block the only route to the next platform).
		const float patrol_min = x - cfg::monster::PATROL_RANGE;
		const float patrol_max = x + hazard_w + cfg::monster::PATROL_RANGE;
		monsters_.push_back(new Monster(*texture, sf::Vector2f(x, y),
		                                monsterHealth(), cfg::monster::SPEED,
		                                patrol_min, patrol_max));
	}
}
