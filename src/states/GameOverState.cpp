// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "states/GameOverState.hpp"

#include "audio/AudioManager.hpp"
#include "core/Constants.hpp"
#include "core/Game.hpp"
#include "core/Settings.hpp"
#include "ui/Layout.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

GameOverState::GameOverState(Game& game) : State(game) {
	// The gameplay's losing sound has already played; make sure no menu music
	// leaks onto this screen.
	game_.audio().stopMusic();

	const float screen_width = static_cast<float>(cfg::window::WIDTH);
	const float screen_height = static_cast<float>(cfg::window::HEIGHT);

	ui::stretchToWindow(background_,
	                    game_.textures().acquire(cfg::assets::BACKGROUND));

	// Freeze the exact frame from the moment the player lost, then dim it with
	// a frosted veil so it is clear WHERE the run ended (TA feedback).
	if (game_.hasLastFrame()) {
		death_frame_.setTexture(game_.lastFrame(), true);
		death_frame_.setPosition(0.0f, 0.0f);
		matte_.setSize(sf::Vector2f(screen_width, screen_height));
		matte_.setPosition(0.0f, 0.0f);
		matte_.setFillColor(cfg::ui::GAMEOVER_MATTE_COLOR);
		has_death_frame_ = true;
	}

	const sf::Font& font = game_.fonts().acquire(cfg::assets::FONT);

	game_over_text_.setFont(font);
	game_over_text_.setString("YOU LOST");
	game_over_text_.setCharacterSize(cfg::ui::GAME_OVER_TEXT_SIZE);
	game_over_text_.setFillColor(cfg::ui::ACCENT_COLOR);
	ui::centerTextHorizontally(game_over_text_, screen_width / 2.0f,
	                           screen_height * 0.16f);

	score_text_.setFont(font);
	score_text_.setString("Score: " + std::to_string(game_.lastScore()));
	score_text_.setCharacterSize(cfg::ui::LABEL_TEXT_SIZE);
	score_text_.setFillColor(cfg::ui::TEXT_COLOR);
	ui::centerTextHorizontally(score_text_, screen_width / 2.0f,
	                           screen_height * 0.30f);

	const Difficulty level = game_.settings().difficulty();
	high_score_text_.setFont(font);
	high_score_text_.setString("Best (" + std::string(difficulty::name(level)) +
	                           "): " + std::to_string(game_.highScore()));
	high_score_text_.setCharacterSize(cfg::ui::LABEL_TEXT_SIZE);
	high_score_text_.setFillColor(cfg::ui::TEXT_COLOR);
	ui::centerTextHorizontally(high_score_text_, screen_width / 2.0f,
	                           screen_height * 0.38f);

	// Celebrate a fresh record: submitScore() already ran, so matching the
	// stored best (with a non-zero score) means this run set or tied it.
	is_new_best_ =
		game_.lastScore() > 0 && game_.lastScore() >= game_.highScore();
	if (is_new_best_) {
		new_best_text_.setFont(font);
		new_best_text_.setString("NEW BEST!");
		new_best_text_.setCharacterSize(cfg::ui::LABEL_TEXT_SIZE);
		new_best_text_.setFillColor(cfg::ui::ACCENT_COLOR);
		ui::centerTextHorizontally(new_best_text_, screen_width / 2.0f,
		                           screen_height * 0.46f);
	}

	restart_button_ = std::make_unique<Button>(
		game_.textures().acquire(cfg::assets::RESTART_BUTTON),
		sf::Vector2f(screen_width / 2.0f, screen_height * 0.58f));
	menu_button_ = std::make_unique<Button>(
		game_.textures().acquire(cfg::assets::MENU_BUTTON),
		sf::Vector2f(screen_width / 2.0f, screen_height * 0.72f));
}

void GameOverState::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed &&
	    event.mouseButton.button == sf::Mouse::Left) {
		const sf::Vector2f point = game_.window().mapPixelToCoords(
			{event.mouseButton.x, event.mouseButton.y},
			game_.window().getDefaultView());
		if (restart_button_->contains(point)) {
			game_.requestState(StateId::Play);
		} else if (menu_button_->contains(point)) {
			game_.requestState(StateId::Menu);
		}
	}
}

void GameOverState::update(float /*dt*/) {}

void GameOverState::render(sf::RenderWindow& window) {
	window.setView(window.getDefaultView());
	// The frozen death scene (dimmed) if we captured it, otherwise the plain
	// background as a fallback.
	if (has_death_frame_) {
		window.draw(death_frame_);
		window.draw(matte_);
	} else {
		window.draw(background_);
	}
	window.draw(game_over_text_);
	window.draw(score_text_);
	window.draw(high_score_text_);
	if (is_new_best_) {
		window.draw(new_best_text_);
	}
	restart_button_->draw(window);
	menu_button_->draw(window);
}
