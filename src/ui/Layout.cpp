// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "ui/Layout.hpp"

#include "core/Constants.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace ui {

void centerTextHorizontally(sf::Text& text, float center_x, float y) {
	const sf::FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top);
	text.setPosition(center_x, y);
}

void stretchToWindow(sf::Sprite& sprite, const sf::Texture& texture) {
	sprite.setTexture(texture);
	const float window_width = static_cast<float>(cfg::window::WIDTH);
	const float window_height = static_cast<float>(cfg::window::HEIGHT);
	sprite.setScale(window_width / texture.getSize().x,
	                window_height / texture.getSize().y);
}

} // namespace ui
