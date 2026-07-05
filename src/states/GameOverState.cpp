// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "states/GameOverState.hpp"

#include "core/Constants.hpp"
#include "core/Game.hpp"
#include "ui/Layout.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

GameOverState::GameOverState(Game& game) : State(game) {
	const float screen_width = static_cast<float>(cfg::window::WIDTH);
	const float screen_height = static_cast<float>(cfg::window::HEIGHT);

	ui::stretchToWindow(background_,
	                    game_.textures().acquire(cfg::assets::BACKGROUND));

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

	high_score_text_.setFont(font);
	high_score_text_.setString("Best: " + std::to_string(game_.highScore()));
	high_score_text_.setCharacterSize(cfg::ui::LABEL_TEXT_SIZE);
	high_score_text_.setFillColor(cfg::ui::TEXT_COLOR);
	ui::centerTextHorizontally(high_score_text_, screen_width / 2.0f,
	                           screen_height * 0.38f);

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
	window.draw(background_);
	window.draw(game_over_text_);
	window.draw(score_text_);
	window.draw(high_score_text_);
	restart_button_->draw(window);
	menu_button_->draw(window);
}
