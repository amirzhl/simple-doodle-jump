// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "entities/Hole.hpp"

#include <SFML/Graphics/Texture.hpp>

Hole::Hole(const sf::Texture& texture, sf::Vector2f position) {
	position_ = position;
	sprite_.setTexture(texture);
	sprite_.setPosition(position_);
}

void Hole::update(float /*dt*/) {
	// A hole is stationary; scrolling is handled by the moving camera view.
}

void Hole::draw(sf::RenderTarget& target) const {
	target.draw(sprite_);
}

sf::FloatRect Hole::getBounds() const {
	return sprite_.getGlobalBounds();
}

sf::Vector2f Hole::center() const {
	const sf::FloatRect bounds = sprite_.getGlobalBounds();
	return sf::Vector2f(bounds.left + bounds.width / 2.0f,
	                    bounds.top + bounds.height / 2.0f);
}

bool Hole::isExpired(float world_bottom) const {
	return getBounds().top > world_bottom;
}
