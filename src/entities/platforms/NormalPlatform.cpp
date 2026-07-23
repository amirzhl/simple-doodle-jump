// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "entities/platforms/NormalPlatform.hpp"

#include "core/Constants.hpp"
#include "entities/Player.hpp"

#include <SFML/Graphics/Texture.hpp>

NormalPlatform::NormalPlatform(const sf::Texture& texture, sf::Vector2f position)
	: Platform(texture, position) {}

void NormalPlatform::attachSpring(const sf::Texture& spring_texture) {
	const sf::FloatRect bounds = getBounds();
	const sf::FloatRect spring_size(0.0f, 0.0f, 25.0f, 26.0f);
	// Centre the spring horizontally on the platform and rest it on the top.
	const float x = bounds.left + (bounds.width - spring_size.width) / 2.0f;
	const float y = bounds.top - spring_size.height;
	spring_ = std::make_unique<Spring>(spring_texture, sf::Vector2f(x, y));
}

void NormalPlatform::update(float dt) {
	Platform::update(dt);
	if (spring_) {
		spring_->update(dt);
	}
}


float NormalPlatform::onLand(Player& player) {
	// The spring only fires when the player actually lands ON the spring:
	// their horizontal spans must overlap. Landing on the bare part of the
	// platform gives a normal jump, exactly like the original game.
	if (spring_) {
		const sf::FloatRect player_bounds = player.getBounds();
		const sf::FloatRect spring_bounds = spring_->getBounds();
		const bool touches_spring =
			player_bounds.left < spring_bounds.left + spring_bounds.width &&
			spring_bounds.left < player_bounds.left + player_bounds.width;
		if (touches_spring) {
			spring_->compress();
			return cfg::physics::SPRING_JUMP;
		}
	}
	return cfg::physics::NORMAL_JUMP;
}

void NormalPlatform::draw(sf::RenderTarget& target) const {
	Platform::draw(target);
	if (spring_) {
		spring_->draw(target);
	}
}
