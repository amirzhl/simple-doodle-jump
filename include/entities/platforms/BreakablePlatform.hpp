// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include "entities/platforms/Platform.hpp"

// ---------------------------------------------------------------------------
//  BreakablePlatform  -- collapses the first time the player lands on it.
//
//  Behaviour once triggered:
//    * no jump is given (onLand returns 0)
//    * the platform starts falling downward
//    * while the player still rests on it, the collision system drags the
//      player down with it (supportVelocity())
//    * the player can steer left/right to reach a healthy platform (rescue)
//
//  A breakable platform is NEVER counted as part of the guaranteed reachable
//  route (see PlatformManager), matching the spec's warning.
// ---------------------------------------------------------------------------
class BreakablePlatform : public Platform {
public:
	BreakablePlatform(const sf::Texture& texture, sf::Vector2f position);

	void update(float dt) override;

	float onLand(Player& player) override;
	bool isSupportive() const override { return !broken_; }
	float supportVelocity() const override;

private:
	bool broken_ = false;
};
