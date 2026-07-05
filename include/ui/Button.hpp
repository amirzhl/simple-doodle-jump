// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf {
class Texture;
class RenderTarget;
} // namespace sf

// ---------------------------------------------------------------------------
//  Button  -- a small, reusable clickable image button used by the menu and
//  game-over screens. It knows how to draw itself and to test whether a point
//  (a mapped mouse position) falls inside it.
// ---------------------------------------------------------------------------
class Button {
public:
	// `center` is the desired centre position of the button in screen space.
	Button(const sf::Texture& texture, sf::Vector2f center);

	void draw(sf::RenderTarget& target) const;

	// True if the given (already view-mapped) point is inside the button.
	bool contains(sf::Vector2f point) const;

private:
	sf::Sprite sprite_;
};
