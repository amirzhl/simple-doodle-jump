// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "ui/Button.hpp"

#include "core/Constants.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>

Button::Button(const sf::Texture& texture, sf::Vector2f center) {
	sprite_.setTexture(texture);
	// Scale to a fixed on-screen width so all buttons look consistent even
	// though their source images have different resolutions.
	const sf::FloatRect local = sprite_.getLocalBounds();
	const float scale = cfg::ui::BUTTON_TARGET_WIDTH / local.width;
	sprite_.setScale(scale, scale);
	// Centre the origin so `center` positions the button by its middle.
	sprite_.setOrigin(local.width / 2.0f, local.height / 2.0f);
	sprite_.setPosition(center);
}

void Button::draw(sf::RenderTarget& target) const {
	target.draw(sprite_);
}

bool Button::contains(sf::Vector2f point) const {
	return sprite_.getGlobalBounds().contains(point);
}
