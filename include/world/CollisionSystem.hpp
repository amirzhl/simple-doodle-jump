// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include <vector>

class Player;
class Platform;
class Monster;

// ---------------------------------------------------------------------------
//  CollisionSystem  -- the ONE, centralised place that resolves player/
//  platform (and player/spring) collisions. Every platform type flows through
//  the exact same detection logic here; the differing *response* is delegated
//  polymorphically to Platform::onLand(), so no landing logic is duplicated
//  per subtype (a hard requirement of the spec).
//
//  Detection uses a swept check on the vertical axis: the player only lands
//  when it is moving downward and its feet cross a platform's top surface
//  between the previous and current frame. This avoids side-collisions and
//  "tunnelling" through thin platforms at high speed.
//
//  Phase 2 reuses the very same swept test for monsters: a top-cross bounces
//  the player, while any other overlap is fatal.
// ---------------------------------------------------------------------------
class CollisionSystem {
public:
	// The outcome of a monster interaction this frame.
	enum class MonsterHit {
		None,     // no contact
		Bounced,  // landed on a monster's head -> strong bounce (survives)
		Killed,   // any other contact -> game over
	};

	// Resolve player/platform collisions for this frame. Returns true if the
	// player performed an upward jump (so the caller can play the jump sound).
	// `dt` is needed to reconstruct the player's previous position.
	bool resolve(Player& player, std::vector<Platform*>& platforms,
	             float dt) const;

	// Resolve player/monster collisions for this frame using the same swept
	// test. On a head-bounce the player is launched upward here.
	MonsterHit resolveMonsters(Player& player,
	                           std::vector<Monster*>& monsters, float dt) const;

private:
	static bool horizontallyOverlaps(const Player& player,
	                                 const Platform& platform);
};
