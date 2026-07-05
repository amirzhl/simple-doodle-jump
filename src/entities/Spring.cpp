// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "entities/Spring.hpp"

#include <SFML/Graphics/Texture.hpp>

namespace {
// The sheet is 25x52: two vertically stacked 25x26 frames.
constexpr int FRAME_WIDTH = 25;
constexpr int FRAME_HEIGHT = 26;
const sf::IntRect EXTENDED_FRAME(0, 0, FRAME_WIDTH, FRAME_HEIGHT);
const sf::IntRect COMPRESSED_FRAME(0, FRAME_HEIGHT, FRAME_WIDTH, FRAME_HEIGHT);
} // namespace

Spring::Spring(const sf::Texture& texture, sf::Vector2f position) {
	sprite_.setTexture(texture);
	sprite_.setTextureRect(EXTENDED_FRAME);
	position_ = position;
	sprite_.setPosition(position_);
}

void Spring::update(float /*dt*/) {
	// The spring itself has no autonomous animation; it only reacts to bounces
	// (compress) and to being moved by its host platform.
	sprite_.setPosition(position_);
}

void Spring::compress() {
	if (!compressed_) {
		compressed_ = true;
		sprite_.setTextureRect(COMPRESSED_FRAME);
	}
}


sf::FloatRect Spring::getBounds() const {
	return sprite_.getGlobalBounds();
}

void Spring::draw(sf::RenderTarget& target) const {
	target.draw(sprite_);
}
