// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include <vector>

template <typename Resource>
class ResourceManager;
namespace sf {
class Texture;
}
class Platform;

// ---------------------------------------------------------------------------
//  PlatformManager  -- owns the collection of live platforms (as a vector of
//  Platform* for polymorphism) and is solely responsible for:
//    * seeding the initial world
//    * procedurally generating new platforms above the view (endless world)
//    * deleting platforms that scroll off the bottom (memory management)
//    * enforcing the accessibility guarantee
//
//  Accessibility guarantee
//  -----------------------
//  Every rung is placed at most MAX_GAP above the previous one, and
//  2*MAX_GAP stays below the maximum normal jump height. Two breakable
//  platforms never appear in a row, so the player can always skip a single
//  breakable by jumping from the safe platform below straight to the safe
//  platform above. Horizontal reachability is guaranteed by screen wrap.
// ---------------------------------------------------------------------------
class PlatformManager {
public:
	explicit PlatformManager(ResourceManager<sf::Texture>& textures);
	~PlatformManager();

	PlatformManager(const PlatformManager&) = delete;
	PlatformManager& operator=(const PlatformManager&) = delete;

	// Clear everything and build a fresh starting world. Returns the top-centre
	// position of the guaranteed starting platform so the player can be placed
	// on top of it.
	void reset();

	// Advance platforms and keep the world populated / trimmed for the current
	// camera window [world_top, world_bottom].
	void update(float dt, float world_top, float world_bottom);

	std::vector<Platform*>& platforms() { return platforms_; }

	// The y of the very first (starting) platform's top surface.
	float startingPlatformTop() const { return starting_platform_top_; }

private:
	void clear();
	void generateAbove(float world_top);
	void removeExpired(float world_bottom);
	Platform* createPlatform(float x, float y, bool allow_breakable);

	// Detect (via dynamic_cast) whether the most recently generated platform is
	// a breakable one, so the next rung is never allowed to be breakable too.
	bool lastPlatformIsBreakable() const;

	ResourceManager<sf::Texture>& textures_;
	std::vector<Platform*> platforms_;
	float highest_y_ = 0.0f;       // y of the highest platform generated so far
	float starting_platform_top_ = 0.0f;
};
