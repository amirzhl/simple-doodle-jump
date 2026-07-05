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
//  GameOverState  -- the "YOU LOST" screen. Shows the final score and the
//  best score, and offers two mouse buttons:
//    * Restart   -> start a new session immediately (no trip through the menu)
//    * Main Menu -> return to the main menu
// ---------------------------------------------------------------------------
class GameOverState : public State {
public:
	explicit GameOverState(Game& game);

	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;

private:
	sf::Sprite background_;
	sf::Text game_over_text_;
	sf::Text score_text_;
	sf::Text high_score_text_;
	std::unique_ptr<Button> restart_button_;
	std::unique_ptr<Button> menu_button_;
};
