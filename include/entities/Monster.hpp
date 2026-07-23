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
//  Monster  -- a flying enemy (one of two random visual variants). It patrols
//  horizontally, bouncing off the screen edges exactly like a MovingPlatform.
//
//  Interaction rules (resolved by the CollisionSystem):
//    * landing on its head  -> the player performs a strong bounce (survives)
//    * any other contact    -> instant game over
//    * a bullet hit         -> loses one health point; dies when it reaches 0
//
//  Its starting health depends on the difficulty (see World).
// ---------------------------------------------------------------------------
class Monster : public Entity {
public:
	// The monster patrols horizontally inside [patrol_min_x, patrol_max_x]
	// (clamped to the screen), so it never blocks the whole route upward.
	Monster(const sf::Texture& texture, sf::Vector2f position, int health,
	        float speed, float patrol_min_x, float patrol_max_x);

	void update(float dt) override;
	void draw(sf::RenderTarget& target) const override;
	sf::FloatRect getBounds() const override;

	// Remove `amount` health (from a bullet hit).
	void takeDamage(int amount);
	bool isAlive() const { return health_ > 0; }

	// True once the monster has scrolled below the visible world.
	bool isExpired(float world_bottom) const;

private:
	void syncSprite();

	sf::Sprite sprite_;
	int health_;
	float speed_;
	float direction_ = 1.0f;
	float patrol_min_x_ = 0.0f;
	float patrol_max_x_ = 0.0f;
};
