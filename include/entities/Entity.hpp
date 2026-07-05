// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

// ---------------------------------------------------------------------------
//  Entity  -- the common abstract base for every drawable, updatable object
//  in the world (the player, every platform, every spring).
//
//  It defines the minimal shared interface the game loop relies on:
//    * update(dt) : advance internal state, frame-rate independent
//    * draw(target): render onto a window / render-target
//    * getBounds(): axis-aligned bounding box used by the collision system
//    * a position accessor shared by all entities
//
//  Because every method the game loop touches is virtual, containers of
//  Entity* / Platform* obtain correct behaviour through dynamic binding.
// ---------------------------------------------------------------------------
class Entity {
public:
	virtual ~Entity() = default;

	// Advance the entity by `dt` seconds (delta time -> frame independent).
	virtual void update(float dt) = 0;

	// Draw the entity onto the given render target.
	virtual void draw(sf::RenderTarget& target) const = 0;

	// World-space axis-aligned bounding box (used for collisions).
	virtual sf::FloatRect getBounds() const = 0;

	sf::Vector2f getPosition() const { return position_; }

	void setPosition(sf::Vector2f position) { position_ = position; }

protected:
	// Top-left position of the entity in world coordinates. Kept in the base
	// class because every entity needs it; derived classes keep their sprite
	// synchronised with this value.
	sf::Vector2f position_{0.0f, 0.0f};
};
