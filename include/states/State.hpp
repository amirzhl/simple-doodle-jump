// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include <SFML/Window/Event.hpp>

namespace sf {
class RenderWindow;
}
class Game;

// ---------------------------------------------------------------------------
//  State  -- abstract base for the three screens (Menu, Play, GameOver).
//  Each state owns its own logic and rendering; the Game drives them through
//  this uniform interface (classic State pattern, resolved via dynamic
//  binding). A state can request a transition through its Game reference.
// ---------------------------------------------------------------------------
class State {
public:
	explicit State(Game& game) : game_(game) {}
	virtual ~State() = default;

	virtual void handleEvent(const sf::Event& event) = 0;
	virtual void update(float dt) = 0;
	virtual void render(sf::RenderWindow& window) = 0;

protected:
	Game& game_;
};
