// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "entities/Bullet.hpp"

#include "core/Constants.hpp"

#include <SFML/Graphics/Texture.hpp>

Bullet::Bullet(const sf::Texture& texture, sf::Vector2f muzzle) {
	sprite_.setTexture(texture);
	// The nose art fills only the top of its 28x120 canvas; crop away the
	// transparent rest so the visual matches the collision bounds (otherwise
	// bullets would "hit" monsters that are visibly far below the nose).
	sprite_.setTextureRect(sf::IntRect(cfg::bullet::ART_LEFT, cfg::bullet::ART_TOP,
	                                   cfg::bullet::ART_WIDTH,
	                                   cfg::bullet::ART_HEIGHT));
	sprite_.setScale(cfg::bullet::SCALE, cfg::bullet::SCALE);
	// Centre the sprite on the muzzle point.
	const sf::FloatRect local = sprite_.getLocalBounds();
	sprite_.setOrigin(local.width / 2.0f, local.height / 2.0f);
	position_ = muzzle;
	sprite_.setPosition(position_);
}

void Bullet::update(float dt) {
	position_.y -= cfg::bullet::SPEED * dt; // travels straight up
	sprite_.setPosition(position_);
}

void Bullet::draw(sf::RenderTarget& target) const {
	target.draw(sprite_);
}

sf::FloatRect Bullet::getBounds() const {
	return sprite_.getGlobalBounds();
}

bool Bullet::isOffscreen(float world_top) const {
	const sf::FloatRect bounds = sprite_.getGlobalBounds();
	return bounds.top + bounds.height < world_top - cfg::bullet::OFFSCREEN_BUFFER;
}
