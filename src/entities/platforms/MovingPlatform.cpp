// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "entities/platforms/MovingPlatform.hpp"

#include "core/Constants.hpp"
#include "utils/Random.hpp"

#include <SFML/Graphics/Texture.hpp>

MovingPlatform::MovingPlatform(const sf::Texture& texture, sf::Vector2f position,
                               float speed)
	: Platform(texture, position), speed_(speed) {
	// Start moving in a random direction.
	direction_ = Random::chance(0.5f) ? 1.0f : -1.0f;
}

void MovingPlatform::update(float dt) {
	const float width = getBounds().width;
	const float screen_width = static_cast<float>(cfg::window::WIDTH);

	position_.x += direction_ * speed_ * dt;

	// Bounce off the screen edges.
	if (position_.x <= 0.0f) {
		position_.x = 0.0f;
		direction_ = 1.0f;
	} else if (position_.x + width >= screen_width) {
		position_.x = screen_width - width;
		direction_ = -1.0f;
	}

	syncSprite();
}
