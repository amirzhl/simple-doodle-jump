// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "states/MenuState.hpp"

#include "audio/AudioManager.hpp"
#include "core/Constants.hpp"
#include "core/Game.hpp"
#include "core/Settings.hpp"
#include "ui/Layout.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

MenuState::MenuState(Game& game) : State(game) {
	const float screen_width = static_cast<float>(cfg::window::WIDTH);
	const float screen_height = static_cast<float>(cfg::window::HEIGHT);

	ui::stretchToWindow(background_,
	                    game_.textures().acquire(cfg::assets::BACKGROUND));

	const sf::Font& font = game_.fonts().acquire(cfg::assets::FONT);

	title_text_.setFont(font);
	title_text_.setString("Doodle Jump");
	title_text_.setCharacterSize(cfg::ui::TITLE_TEXT_SIZE);
	title_text_.setFillColor(cfg::ui::ACCENT_COLOR);
	ui::centerTextHorizontally(title_text_, screen_width / 2.0f,
	                           screen_height * 0.18f);

	// Best score is tracked per difficulty; show the active difficulty's best.
	const Difficulty level = game_.settings().difficulty();
	high_score_text_.setFont(font);
	high_score_text_.setString("Best (" + std::string(difficulty::name(level)) +
	                           "): " + std::to_string(game_.highScore()));
	high_score_text_.setCharacterSize(cfg::ui::LABEL_TEXT_SIZE);
	high_score_text_.setFillColor(cfg::ui::TEXT_COLOR);
	ui::centerTextHorizontally(high_score_text_, screen_width / 2.0f,
	                           screen_height * 0.32f);

	start_button_ = std::make_unique<Button>(
		game_.textures().acquire(cfg::assets::START_BUTTON),
		sf::Vector2f(screen_width / 2.0f, screen_height * 0.55f));
	settings_button_ = std::make_unique<Button>(
		game_.textures().acquire(cfg::assets::SETTINGS_BUTTON),
		sf::Vector2f(screen_width / 2.0f, screen_height * 0.70f));

	// Controls hint requested by the TA: make the movement keys explicit.
	hint_text_.setFont(font);
	hint_text_.setString("Use Left / Right arrows to move");
	hint_text_.setCharacterSize(cfg::ui::HINT_TEXT_SIZE);
	hint_text_.setFillColor(cfg::ui::TEXT_COLOR);
	ui::centerTextHorizontally(hint_text_, screen_width / 2.0f,
	                           screen_height * 0.88f);

	// Start (or keep) the looping menu music.
	game_.audio().playMenuMusic();
}

void MenuState::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed &&
	    event.mouseButton.button == sf::Mouse::Left) {
		const sf::Vector2f point = game_.window().mapPixelToCoords(
			{event.mouseButton.x, event.mouseButton.y},
			game_.window().getDefaultView());
		if (start_button_->contains(point)) {
			game_.requestState(StateId::Play);
		} else if (settings_button_->contains(point)) {
			game_.requestState(StateId::Settings);
		}
	}
}

void MenuState::update(float /*dt*/) {}

void MenuState::render(sf::RenderWindow& window) {
	window.setView(window.getDefaultView());
	window.draw(background_);
	window.draw(title_text_);
	window.draw(high_score_text_);
	start_button_->draw(window);
	settings_button_->draw(window);
	window.draw(hint_text_);
}
