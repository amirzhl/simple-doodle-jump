// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "entities/Player.hpp"

#include "core/Constants.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cmath>

Player::Player(const sf::Texture& left_texture, const sf::Texture& right_texture)
	: left_texture_(left_texture), right_texture_(right_texture) {
	sprite_.setTexture(right_texture_);
	sprite_.setScale(cfg::player::SCALE, cfg::player::SCALE);
}

void Player::reset(sf::Vector2f start_position) {
	position_ = start_position;
	velocity_ = {0.0f, 0.0f};
	start_y_ = start_position.y;
	highest_y_ = start_position.y;
	score_ = 0;
	sprite_.setTexture(right_texture_);
	syncSprite();
}

void Player::handleInput() {
	velocity_.x = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		velocity_.x = -cfg::player::MOVE_SPEED;
		sprite_.setTexture(left_texture_);
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		velocity_.x = cfg::player::MOVE_SPEED;
		sprite_.setTexture(right_texture_);
	}
}

void Player::update(float dt) {
	// Gravity continuously increases the downward velocity (frame independent).
	velocity_.y += cfg::physics::GRAVITY * dt;

	// Integrate motion.
	position_.x += velocity_.x * dt;
	position_.y += velocity_.y * dt;

	wrapHorizontally();
	updateScore();
	syncSprite();
}

void Player::jump(float vertical_velocity) {
	velocity_.y = vertical_velocity;
}

void Player::setFeetY(float y) {
	const sf::FloatRect bounds = sprite_.getGlobalBounds();
	position_.y = y - bounds.height;
	syncSprite();
}

sf::FloatRect Player::getBounds() const {
	return sprite_.getGlobalBounds();
}

void Player::draw(sf::RenderTarget& target) const {
	target.draw(sprite_);
}

void Player::wrapHorizontally() {
	const float width = sprite_.getGlobalBounds().width;
	const float screen_width = static_cast<float>(cfg::window::WIDTH);
	if (position_.x + width < 0.0f) {
		position_.x = screen_width;
	} else if (position_.x > screen_width) {
		position_.x = -width;
	}
}

void Player::updateScore() {
	if (position_.y < highest_y_) {
		highest_y_ = position_.y;
	}
	score_ = static_cast<int>((start_y_ - highest_y_) / cfg::player::PIXELS_PER_POINT);
	if (score_ < 0) {
		score_ = 0;
	}
}

void Player::syncSprite() {
	sprite_.setPosition(position_);
}
