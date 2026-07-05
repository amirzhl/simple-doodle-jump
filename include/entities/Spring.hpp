// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include "entities/Entity.hpp"

#include <SFML/Graphics/Sprite.hpp>

namespace sf {
class Texture;
}

// ---------------------------------------------------------------------------
//  Spring  -- an optional power-up that sits on top of a normal platform and
//  launches the player far higher than a normal jump.
//
//  The single sprite sheet (spring_sprite.png) holds two 25x26 frames stacked
//  vertically: the top frame is the extended (resting) spring, the bottom
//  frame is the compressed one. We switch frames with setTextureRect, exactly
//  as suggested by the teaching assistants.
// ---------------------------------------------------------------------------
class Spring : public Entity {
public:
	Spring(const sf::Texture& texture, sf::Vector2f position);

	void update(float dt) override;
	void draw(sf::RenderTarget& target) const override;
	sf::FloatRect getBounds() const override;

	// Switch to the compressed frame (called when the player bounces on it).
	void compress();


private:
	sf::Sprite sprite_;
	bool compressed_ = false;
};
