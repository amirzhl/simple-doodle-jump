// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "states/SettingsState.hpp"

#include "audio/AudioManager.hpp"
#include "core/Constants.hpp"
#include "core/Game.hpp"
#include "core/Settings.hpp"
#include "ui/Layout.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

namespace {
bool rectContains(const sf::FloatRect& rect, sf::Vector2f point) {
	return rect.contains(point);
}
} // namespace

SettingsState::SettingsState(Game& game)
	: State(game),
	  volume_slider_(
		  sf::Vector2f(static_cast<float>(cfg::window::WIDTH) / 2.0f -
	                       cfg::ui::SLIDER_WIDTH / 2.0f,
	                   static_cast<float>(cfg::window::HEIGHT) * 0.40f),
		  cfg::ui::SLIDER_WIDTH, cfg::ui::SLIDER_HEIGHT, cfg::audio::MIN_VOLUME,
		  cfg::audio::MAX_VOLUME) {
	const float screen_width = static_cast<float>(cfg::window::WIDTH);
	const float screen_height = static_cast<float>(cfg::window::HEIGHT);

	ui::stretchToWindow(background_,
	                    game_.textures().acquire(cfg::assets::BACKGROUND));

	const sf::Font& font = game_.fonts().acquire(cfg::assets::FONT);

	title_text_.setFont(font);
	title_text_.setString("Settings");
	title_text_.setCharacterSize(cfg::ui::TITLE_TEXT_SIZE);
	title_text_.setFillColor(cfg::ui::ACCENT_COLOR);
	ui::centerTextHorizontally(title_text_, screen_width / 2.0f,
	                           screen_height * 0.12f);

	volume_label_.setFont(font);
	volume_label_.setCharacterSize(cfg::ui::LABEL_TEXT_SIZE);
	volume_label_.setFillColor(cfg::ui::TEXT_COLOR);

	volume_slider_.setValue(game_.settings().volume());
	refreshVolumeLabel();

	difficulty_label_.setFont(font);
	difficulty_label_.setString("Difficulty");
	difficulty_label_.setCharacterSize(cfg::ui::LABEL_TEXT_SIZE);
	difficulty_label_.setFillColor(cfg::ui::TEXT_COLOR);
	ui::centerTextHorizontally(difficulty_label_, screen_width / 2.0f,
	                           screen_height * 0.54f);

	// Three evenly spaced difficulty choices.
	const float option_y = screen_height * 0.64f;
	for (int i = 0; i < difficulty::COUNT; ++i) {
		sf::Text& option = difficulty_options_[static_cast<std::size_t>(i)];
		option.setFont(font);
		option.setString(difficulty::name(difficulty::fromIndex(i)));
		option.setCharacterSize(cfg::ui::LABEL_TEXT_SIZE);
		const float center_x = screen_width * (0.25f + 0.25f * i);
		ui::centerTextHorizontally(option, center_x, option_y);
	}
	refreshDifficultyColors();

	back_button_ = std::make_unique<Button>(
		game_.textures().acquire(cfg::assets::BACK_BUTTON),
		sf::Vector2f(screen_width / 2.0f, screen_height * 0.82f));

	// Music continues on the settings screen just like on the menu.
	game_.audio().playMenuMusic();
}

sf::Vector2f SettingsState::mapMouse(int x, int y) const {
	return game_.window().mapPixelToCoords({x, y},
	                                       game_.window().getDefaultView());
}

void SettingsState::refreshVolumeLabel() {
	volume_label_.setString("Volume: " +
	                        std::to_string(volume_slider_.value()) + "%");
	// Re-centre after every change: the text width varies with the digits.
	ui::centerTextHorizontally(volume_label_,
	                           static_cast<float>(cfg::window::WIDTH) / 2.0f,
	                           static_cast<float>(cfg::window::HEIGHT) * 0.30f);
}

void SettingsState::refreshDifficultyColors() {
	const int current = difficulty::toIndex(game_.settings().difficulty());
	for (int i = 0; i < difficulty::COUNT; ++i) {
		difficulty_options_[static_cast<std::size_t>(i)].setFillColor(
			i == current ? cfg::ui::ACCENT_COLOR : cfg::ui::MUTED_COLOR);
	}
}

void SettingsState::applyDifficulty(Difficulty level) {
	game_.settings().setDifficulty(level);
	refreshDifficultyColors();
}

void SettingsState::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseButtonPressed &&
	    event.mouseButton.button == sf::Mouse::Left) {
		const sf::Vector2f point =
			mapMouse(event.mouseButton.x, event.mouseButton.y);

		if (back_button_->contains(point)) {
			game_.saveSettings(); // persist volume + difficulty
			game_.requestState(StateId::Menu);
			return;
		}

		for (int i = 0; i < difficulty::COUNT; ++i) {
			const sf::Text& option =
				difficulty_options_[static_cast<std::size_t>(i)];
			if (rectContains(option.getGlobalBounds(), point)) {
				applyDifficulty(difficulty::fromIndex(i));
				return;
			}
		}

		if (volume_slider_.handleMousePress(point)) {
			const int v = volume_slider_.value();
			game_.settings().setVolume(v);
			game_.audio().setVolume(v);
			refreshVolumeLabel();
		}
	} else if (event.type == sf::Event::MouseMoved) {
		if (volume_slider_.isDragging()) {
			const sf::Vector2f point =
				mapMouse(event.mouseMove.x, event.mouseMove.y);
			volume_slider_.handleMouseMove(point);
			const int v = volume_slider_.value();
			game_.settings().setVolume(v);
			game_.audio().setVolume(v);
			refreshVolumeLabel();
		}
	} else if (event.type == sf::Event::MouseButtonReleased &&
	           event.mouseButton.button == sf::Mouse::Left) {
		volume_slider_.endDrag();
	}
}

void SettingsState::update(float /*dt*/) {}

void SettingsState::render(sf::RenderWindow& window) {
	window.setView(window.getDefaultView());
	window.draw(background_);
	window.draw(title_text_);
	window.draw(volume_label_);
	volume_slider_.draw(window);
	window.draw(difficulty_label_);
	for (const sf::Text& option : difficulty_options_) {
		window.draw(option);
	}
	back_button_->draw(window);
}
