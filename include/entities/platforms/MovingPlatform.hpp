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
//  MovingPlatform  -- slides horizontally and bounces off the screen edges.
//  It offers a standard jump (inherited from Platform::onLand) and is always
//  a "safe" platform, so it counts towards the reachable route.
// ---------------------------------------------------------------------------
class MovingPlatform : public Platform {
public:
	MovingPlatform(const sf::Texture& texture, sf::Vector2f position);

	void update(float dt) override;

private:
	float direction_ = 1.0f; // +1 = moving right, -1 = moving left
};
