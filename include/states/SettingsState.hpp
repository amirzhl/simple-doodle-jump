// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include "core/Difficulty.hpp"
#include "states/State.hpp"
#include "ui/Button.hpp"
#include "ui/Slider.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <array>
#include <memory>

// ---------------------------------------------------------------------------
//  SettingsState  -- reachable ONLY from the main menu (never during play).
//  It lets the player:
//    * drag a slider to set the master volume (applied live to all audio)
//    * pick one of three difficulties (Easy / Medium / Hard)
//  Pressing Back saves the choices to settings.txt and returns to the menu.
//  Background music keeps playing on this screen (as on the menu).
// ---------------------------------------------------------------------------
class SettingsState : public State {
public:
	explicit SettingsState(Game& game);

	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;

private:
	void applyDifficulty(Difficulty level);
	void refreshVolumeLabel();
	void refreshDifficultyColors();
	sf::Vector2f mapMouse(int x, int y) const;

	sf::Sprite background_;
	sf::Text title_text_;
	sf::Text volume_label_;
	Slider volume_slider_;
	sf::Text difficulty_label_;
	std::array<sf::Text, difficulty::COUNT> difficulty_options_;
	std::unique_ptr<Button> back_button_;
};
