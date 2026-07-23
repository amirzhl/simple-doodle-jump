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
//  Bullet  -- a projectile fired straight up from the player's nose. It moves
//  at a constant speed and interacts ONLY with monsters (it passes through
//  platforms, springs and holes). PlayState deletes it once it leaves the top
//  of the view or hits a monster, so no bullet is ever leaked.
//
//  `position_` is used here as the CENTRE of the projectile so it lines up
//  neatly with the muzzle.
// ---------------------------------------------------------------------------
class Bullet : public Entity {
public:
	Bullet(const sf::Texture& texture, sf::Vector2f muzzle);

	void update(float dt) override;
	void draw(sf::RenderTarget& target) const override;
	sf::FloatRect getBounds() const override;

	// True once the whole bullet has travelled above the top of the view.
	bool isOffscreen(float world_top) const;

private:
	sf::Sprite sprite_;
};
