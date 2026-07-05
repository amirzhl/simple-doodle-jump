// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "entities/platforms/Platform.hpp"

#include "core/Constants.hpp"
#include "entities/Player.hpp"

#include <SFML/Graphics/Texture.hpp>

Platform::Platform(const sf::Texture& texture, sf::Vector2f position) {
	sprite_.setTexture(texture);
	position_ = position;
	syncSprite();
}

void Platform::update(float /*dt*/) {
	// Base platforms are static; subtypes override this when they move.
	syncSprite();
}


float Platform::onLand(Player& /*player*/) {
	// Standard, stable jump. Moving platforms reuse this behaviour directly;
	// Normal (with spring) and Breakable platforms override it.
	return cfg::physics::NORMAL_JUMP;
}

bool Platform::isExpired(float world_bottom) const {
	return getBounds().top > world_bottom + cfg::world::REMOVAL_BUFFER;
}

sf::FloatRect Platform::getBounds() const {
	return sprite_.getGlobalBounds();
}

void Platform::draw(sf::RenderTarget& target) const {
	target.draw(sprite_);
}

void Platform::syncSprite() {
	sprite_.setPosition(position_);
}
