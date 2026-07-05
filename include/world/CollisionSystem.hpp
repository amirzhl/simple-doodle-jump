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
// ---------------------------------------------------------------------------
class CollisionSystem {
public:
	// Resolve collisions for this frame. `dt` is the elapsed time, needed to
	// reconstruct the player's previous position for the swept test.
	void resolve(Player& player, std::vector<Platform*>& platforms,
	             float dt) const;

private:
	static bool horizontallyOverlaps(const Player& player,
	                                 const Platform& platform);
};
