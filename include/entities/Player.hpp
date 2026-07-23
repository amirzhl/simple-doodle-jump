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
//  Player  -- the single controllable character.
//
//  Responsibilities (kept deliberately narrow to respect SRP):
//    * hold position, velocity and score
//    * apply gravity and integrate motion using delta time
//    * read keyboard input for horizontal movement
//    * wrap around the screen edges
//    * expose jump() so the collision system can launch it upward
//    * (phase 2) switch to a shooting pose while firing and expose the muzzle
//      point where bullets are spawned
//    * (phase 2) support the hole "suction" animation via setCenter/
//      setVisualScale, letting PlayState shrink the player into a hole
//
//  It does NOT know about platforms, springs, monsters or bullets; those live
//  in their own classes, which merely call jump()/getBounds()/etc.
// ---------------------------------------------------------------------------
class Player : public Entity {
public:
	Player(const sf::Texture& left_texture, const sf::Texture& right_texture,
	       const sf::Texture& shooting_texture);

	// Read the arrow keys and set the horizontal velocity accordingly.
	void handleInput();

	// Apply gravity, integrate motion, wrap horizontally, update score.
	void update(float dt) override;

	void draw(sf::RenderTarget& target) const override;
	sf::FloatRect getBounds() const override;

	// Launch the player upward with the given (negative) vertical velocity.
	void jump(float vertical_velocity);

	// Reset to a starting position for a brand new game session.
	void reset(sf::Vector2f start_position);

	float getVerticalVelocity() const { return velocity_.y; }
	void setVerticalVelocity(float vy) { velocity_.y = vy; }

	// Snap the player so that the soles of its feet rest on `y`.
	void setFeetY(float y);

	int getScore() const { return score_; }

	// ---- shooting (phase 2) ----
	// Enter / leave the shooting pose. While shooting, the sprite shows the
	// dedicated "shooting" texture regardless of movement direction.
	void startShooting();
	void stopShooting();
	bool isShooting() const { return shooting_; }

	// World-space point (top centre of the sprite) where bullets appear.
	sf::Vector2f getMuzzlePosition() const;

	// ---- hole suction animation (phase 2) ----
	// Move the player so that its centre sits at `center`.
	void setCenter(sf::Vector2f center);
	// Scale the sprite uniformly (used to shrink into a hole).
	void setVisualScale(float scale);

private:
	void syncSprite();
	void wrapHorizontally();
	void updateScore();
	void refreshDirectionTexture();
	// Swap the sprite texture (resetting its rect) while keeping the feet
	// anchored, because the doodle textures have different sizes.
	void switchTexture(const sf::Texture& texture);

	const sf::Texture& left_texture_;
	const sf::Texture& right_texture_;
	const sf::Texture& shooting_texture_;
	sf::Sprite sprite_;

	sf::Vector2f velocity_{0.0f, 0.0f};
	float start_y_ = 0.0f;   // vertical position when the session began
	float highest_y_ = 0.0f; // smallest y (= greatest height) ever reached
	int score_ = 0;
	bool shooting_ = false;
	bool facing_right_ = true;
};
