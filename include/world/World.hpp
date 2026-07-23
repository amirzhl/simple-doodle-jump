// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include "core/Difficulty.hpp"
#include "world/PlatformManager.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <vector>

template <typename Resource>
class ResourceManager;
namespace sf {
class Texture;
}
class Platform;
class Monster;
class Hole;

// ---------------------------------------------------------------------------
//  World  -- the level coordinator. It COMPOSES the phase-1 PlatformManager
//  (unchanged in spirit) and adds the phase-2 side hazards: monsters and
//  holes. Keeping platforms in their own manager preserves single
//  responsibility, while World owns the cross-cutting concerns the new
//  hazards need:
//
//    * one shared overlap test, areaIsFree(), that every hazard placement
//      consults against ALL live objects (platforms + monsters + holes) so
//      nothing is ever generated on top of anything else (spec 7.1);
//    * a spawn schedule that never fires at the very start of a run and only
//      ever places hazards BESIDE a platform, so a hazard can never become the
//      sole obstacle on the safe route;
//    * difficulty rules: monster health, faster moving platforms, and holes
//      that exist on Hard only.
// ---------------------------------------------------------------------------
class World {
public:
	World(ResourceManager<sf::Texture>& textures, Difficulty difficulty);
	~World();

	World(const World&) = delete;
	World& operator=(const World&) = delete;

	// Rebuild a fresh world for a new session.
	void reset();

	// Advance platforms + hazards and keep them populated / trimmed for the
	// current camera window [world_top, world_bottom].
	void update(float dt, float world_top, float world_bottom);

	std::vector<Platform*>& platforms() { return platform_manager_.platforms(); }
	std::vector<Monster*>& monsters() { return monsters_; }
	std::vector<Hole*>& holes() { return holes_; }

	float startingPlatformTop() const {
		return platform_manager_.startingPlatformTop();
	}

	// THE single overlap authority: true if `candidate` (expanded by a small
	// padding) touches no live platform, monster or hole. Every hazard
	// placement routes through this one function.
	bool areaIsFree(const sf::FloatRect& candidate) const;

private:
	void updateMonsters(float dt, float world_bottom);
	void updateHoles(float world_bottom);
	void maybeSpawnHazard();
	int monsterHealth() const;
	static float movingSpeedFor(Difficulty difficulty);

	ResourceManager<sf::Texture>& textures_;
	Difficulty difficulty_;
	PlatformManager platform_manager_;
	std::vector<Monster*> monsters_;
	std::vector<Hole*> holes_;
	float elapsed_ = 0.0f;      // time since the session started
	float spawn_timer_ = 0.0f;  // countdown to the next hazard attempt
};
