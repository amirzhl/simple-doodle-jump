// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "states/PlayState.hpp"

#include "core/Constants.hpp"
#include "core/Game.hpp"
#include "entities/platforms/Platform.hpp"
#include "ui/Layout.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

PlayState::PlayState(Game& game) : State(game), platforms_(game.textures()) {
	const float screen_width = static_cast<float>(cfg::window::WIDTH);
	const float screen_height = static_cast<float>(cfg::window::HEIGHT);

	// Background is drawn in the fixed HUD view so it never scrolls.
	ui::stretchToWindow(background_,
	                    game_.textures().acquire(cfg::assets::BACKGROUND));

	// Build the world, then place the player on top of the starting platform.
	platforms_.reset();
	player_ = std::make_unique<Player>(
		game_.textures().acquire(cfg::assets::LEFT_DOODLE),
		game_.textures().acquire(cfg::assets::RIGHT_DOODLE));
	const sf::FloatRect player_bounds = player_->getBounds();
	player_->reset(sf::Vector2f(
		screen_width / 2.0f - player_bounds.width / 2.0f,
		platforms_.startingPlatformTop() - player_bounds.height));
	// Give the very first automatic jump so the player launches immediately.
	player_->jump(cfg::physics::NORMAL_JUMP);

	world_view_.setSize(screen_width, screen_height);
	world_view_.setCenter(screen_width / 2.0f, screen_height / 2.0f);

	const sf::Font& font = game_.fonts().acquire(cfg::assets::FONT);
	score_text_.setFont(font);
	score_text_.setCharacterSize(cfg::ui::SCORE_TEXT_SIZE);
	score_text_.setFillColor(cfg::ui::TEXT_COLOR);
	score_text_.setPosition(14.0f, 10.0f);
}

void PlayState::handleEvent(const sf::Event& /*event*/) {
	// Continuous movement is polled directly in update(); no discrete events.
}

void PlayState::update(float dt) {
	player_->handleInput();
	player_->update(dt);

	// The camera window in world coordinates, derived from the scrolling view.
	const float world_top = world_view_.getCenter().y -
	                        static_cast<float>(cfg::window::HEIGHT) / 2.0f;
	const float world_bottom = world_view_.getCenter().y +
	                           static_cast<float>(cfg::window::HEIGHT) / 2.0f;
	platforms_.update(dt, world_top, world_bottom);

	// Resolve landings/bounces AFTER motion ("move in y, then handle y").
	collision_.resolve(*player_, platforms_.platforms(), dt);

	updateCamera();

	score_text_.setString("Score: " + std::to_string(player_->getScore()));

	if (playerHasFallen()) {
		game_.submitScore(player_->getScore());
		game_.requestState(StateId::GameOver);
	}
}

void PlayState::updateCamera() {
	// Follow the player upward only: the view centre never descends again.
	const sf::FloatRect bounds = player_->getBounds();
	const float player_center_y = bounds.top + bounds.height / 2.0f;
	if (player_center_y < world_view_.getCenter().y) {
		world_view_.setCenter(world_view_.getCenter().x, player_center_y);
	}
}

bool PlayState::playerHasFallen() const {
	const float view_bottom = world_view_.getCenter().y +
	                          static_cast<float>(cfg::window::HEIGHT) / 2.0f;
	return player_->getBounds().top > view_bottom;
}

void PlayState::render(sf::RenderWindow& window) {
	// 1) Fixed background in the default view.
	window.setView(window.getDefaultView());
	window.draw(background_);

	// 2) The scrolling world.
	window.setView(world_view_);
	for (const Platform* platform : platforms_.platforms()) {
		platform->draw(window);
	}
	player_->draw(window);

	// 3) The HUD (score) back in the fixed view.
	drawHud(window);
}

void PlayState::drawHud(sf::RenderWindow& window) const {
	window.setView(window.getDefaultView());
	window.draw(score_text_);
}
