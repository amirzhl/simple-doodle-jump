// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include "states/State.hpp"
#include "ui/Button.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <memory>

// ---------------------------------------------------------------------------
//  MenuState  -- the main menu: game title, the best score for the currently
//  selected difficulty, a Start button that begins a new session, and a
//  Settings button that opens the settings screen. The menu also starts the
//  looping background music.
// ---------------------------------------------------------------------------
class MenuState : public State {
public:
	explicit MenuState(Game& game);

	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;

private:
	sf::Sprite background_;
	sf::Text title_text_;
	sf::Text high_score_text_;
	sf::Text hint_text_; // "Use Left / Right arrows to move"
	std::unique_ptr<Button> start_button_;
	std::unique_ptr<Button> settings_button_;
};
