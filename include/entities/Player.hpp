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
//
//  It does NOT know about platforms, springs or collision detection; those
//  live in the CollisionSystem, which merely calls jump()/getBounds()/etc.
// ---------------------------------------------------------------------------
class Player : public Entity {
public:
	Player(const sf::Texture& left_texture, const sf::Texture& right_texture);

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

private:
	void syncSprite();
	void wrapHorizontally();
	void updateScore();

	const sf::Texture& left_texture_;
	const sf::Texture& right_texture_;
	sf::Sprite sprite_;

	sf::Vector2f velocity_{0.0f, 0.0f};
	float start_y_ = 0.0f;   // vertical position when the session began
	float highest_y_ = 0.0f; // smallest y (= greatest height) ever reached
	int score_ = 0;
};
