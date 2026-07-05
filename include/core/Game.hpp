// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include "core/ResourceManager.hpp"
#include "states/StateId.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <memory>

class State;

// ---------------------------------------------------------------------------
//  Game  -- the application root. It owns the window, the shared resource
//  caches, the persistent high score, and the currently active State. It runs
//  the three-phase game loop (process events -> update -> render) and performs
//  safe, deferred state transitions.
// ---------------------------------------------------------------------------
class Game {
public:
	Game();
	// Declared here and defined in Game.cpp, where State is a complete type
	// (required because we hold a std::unique_ptr<State> to a forward-declared
	// class).
	~Game();

	// Enter the main loop; returns when the window is closed.
	void run();

	// Request a transition to another state. The switch is applied safely at
	// the end of the current frame (so a state never deletes itself mid-call).
	void requestState(StateId id);

	// ---- context shared with the states ----
	sf::RenderWindow& window() { return window_; }
	ResourceManager<sf::Texture>& textures() { return textures_; }
	ResourceManager<sf::Font>& fonts() { return fonts_; }

	int highScore() const { return high_score_; }
	int lastScore() const { return last_score_; }

	// Record a finished session's score; updates and persists the high score.
	void submitScore(int score);

private:
	void processEvents();
	void update(float dt);
	void render();
	void applyPendingState();
	std::unique_ptr<State> createState(StateId id);

	void loadHighScore();
	void saveHighScore() const;

	sf::RenderWindow window_;
	ResourceManager<sf::Texture> textures_;
	ResourceManager<sf::Font> fonts_;

	std::unique_ptr<State> current_state_;
	StateId pending_state_ = StateId::Menu;
	bool has_pending_state_ = false;

	int high_score_ = 0;
	int last_score_ = 0;
};
