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
//  MenuState  -- the main menu: game title, the best score so far, and a
//  Start button that begins a new gameplay session on a mouse click.
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
	std::unique_ptr<Button> start_button_;
};
