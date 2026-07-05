// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "entities/platforms/BreakablePlatform.hpp"

#include "core/Constants.hpp"
#include "entities/Player.hpp"

#include <SFML/Graphics/Texture.hpp>

BreakablePlatform::BreakablePlatform(const sf::Texture& texture,
                                     sf::Vector2f position)
	: Platform(texture, position) {}

void BreakablePlatform::update(float dt) {
	if (broken_) {
		// Once broken the platform simply falls straight down.
		position_.y += cfg::platform::BREAKABLE_FALL_SPEED * dt;
	}
	syncSprite();
}

float BreakablePlatform::onLand(Player& /*player*/) {
	// The platform breaks and gives NO jump; it begins to fall instead.
	broken_ = true;
	return 0.0f;
}

float BreakablePlatform::supportVelocity() const {
	// While broken, a player resting on it is dragged down at the fall speed.
	return broken_ ? cfg::platform::BREAKABLE_FALL_SPEED : 0.0f;
}
