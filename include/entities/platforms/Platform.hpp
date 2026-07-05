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
class Player;

// ---------------------------------------------------------------------------
//  Platform  -- abstract base for every kind of platform (Normal, Moving,
//  Breakable). It centralises the behaviour shared by all platforms so the
//  collision system never has to special-case a subtype:
//    * holding a sprite / position and rendering it
//    * exposing a bounding box
//    * scrolling with the world
//    * a virtual onLand() hook that returns the upward impulse to give the
//      player (0 = no jump), which each subtype overrides polymorphically.
//
//  Springs are only ever attached to normal platforms, so the spring itself
//  lives in NormalPlatform, not here.
// ---------------------------------------------------------------------------
class Platform : public Entity {
public:
	Platform(const sf::Texture& texture, sf::Vector2f position);
	~Platform() override = default;

	void update(float dt) override;
	void draw(sf::RenderTarget& target) const override;
	sf::FloatRect getBounds() const override;


	// Called by the collision system when the player lands on the platform's
	// top surface while falling. Returns the upward velocity to apply to the
	// player (a negative number). Returning 0 means "no jump".
	// The base implementation performs a standard jump.
	virtual float onLand(Player& player);

	// Whether the platform can currently support / bounce the player. A broken
	// breakable platform returns false so the collision system stops treating
	// it as a launch pad and instead drags the player down with it.
	virtual bool isSupportive() const { return true; }

	// Vertical velocity the platform imposes on a player riding it (used for
	// the breakable-platform "drag" behaviour). 0 for ordinary platforms.
	virtual float supportVelocity() const { return 0.0f; }

	// True once the platform has scrolled off the bottom / finished its life
	// and should be deleted by the PlatformManager.
	virtual bool isExpired(float world_bottom) const;

protected:
	sf::Sprite sprite_;

	// Keep the sprite's screen position in sync with position_.
	void syncSprite();
};
