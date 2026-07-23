// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "entities/Monster.hpp"

#include "core/Constants.hpp"
#include "utils/Random.hpp"

#include <SFML/Graphics/Texture.hpp>

Monster::Monster(const sf::Texture& texture, sf::Vector2f position, int health,
                 float speed, float patrol_min_x, float patrol_max_x)
	: health_(health), speed_(speed), patrol_min_x_(patrol_min_x),
	  patrol_max_x_(patrol_max_x) {
	position_ = position;
	sprite_.setTexture(texture);
	sprite_.setScale(cfg::monster::SCALE, cfg::monster::SCALE);
	// A random initial direction so patrols do not all look identical.
	direction_ = Random::chance(0.5f) ? 1.0f : -1.0f;
	syncSprite();
}

void Monster::update(float dt) {
	const float width = getBounds().width;
	const float screen_width = static_cast<float>(cfg::window::WIDTH);

	// The patrol corridor is clamped to the screen and always kept wide
	// enough to hold the monster itself.
	float min_x = patrol_min_x_ < 0.0f ? 0.0f : patrol_min_x_;
	float max_x = patrol_max_x_ > screen_width ? screen_width : patrol_max_x_;
	if (max_x - min_x < width) {
		min_x = 0.0f;
		max_x = screen_width;
	}

	position_.x += direction_ * speed_ * dt;

	// Bounce off the corridor edges (same behaviour as a moving platform).
	if (position_.x <= min_x) {
		position_.x = min_x;
		direction_ = 1.0f;
	} else if (position_.x + width >= max_x) {
		position_.x = max_x - width;
		direction_ = -1.0f;
	}

	syncSprite();
}

void Monster::draw(sf::RenderTarget& target) const {
	target.draw(sprite_);
}

sf::FloatRect Monster::getBounds() const {
	return sprite_.getGlobalBounds();
}

void Monster::takeDamage(int amount) {
	health_ -= amount;
}

bool Monster::isExpired(float world_bottom) const {
	return getBounds().top > world_bottom;
}

void Monster::syncSprite() {
	sprite_.setPosition(position_);
}
