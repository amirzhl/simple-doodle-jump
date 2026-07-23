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
//  Hole  -- a stationary suction trap that only appears on Hard. Two random
//  sizes exist (chosen by which texture is passed in). It does nothing on its
//  own; PlayState detects contact and drives the "suck the player in" ending,
//  freezing the rest of the world while the player is pulled to center().
// ---------------------------------------------------------------------------
class Hole : public Entity {
public:
	Hole(const sf::Texture& texture, sf::Vector2f position);

	void update(float dt) override;
	void draw(sf::RenderTarget& target) const override;
	sf::FloatRect getBounds() const override;

	// Centre of the hole in world space (target of the suction animation).
	sf::Vector2f center() const;

	// True once the hole has scrolled below the visible world.
	bool isExpired(float world_bottom) const;

private:
	sf::Sprite sprite_;
};
