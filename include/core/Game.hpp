// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include "audio/AudioManager.hpp"
#include "core/Difficulty.hpp"
#include "core/ResourceManager.hpp"
#include "core/Settings.hpp"
#include "states/StateId.hpp"

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <array>
#include <memory>

class State;

// ---------------------------------------------------------------------------
//  Game  -- the application root. It owns the window, the shared resource
//  caches, the audio manager, the persistent settings, the per-difficulty
//  high scores, and the currently active State. It runs the three-phase game
//  loop (process events -> update -> render) and performs safe, deferred
//  state transitions.
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
	AudioManager& audio() { return audio_; }
	Settings& settings() { return settings_; }

	// Best score for the currently selected difficulty.
	int highScore() const;
	int lastScore() const { return last_score_; }

	// Record a finished session's score; updates and persists the best score
	// for the current difficulty.
	void submitScore(int score);

	// Persist the current settings (called when leaving the settings screen).
	void saveSettings() const;

	// The final gameplay frame ("where you lost"), captured when a session ends
	// so the game-over screen can show it, frosted, behind its UI.
	void setLastFrame(const sf::Texture& frame) {
		last_frame_ = frame;
		has_last_frame_ = true;
	}
	const sf::Texture& lastFrame() const { return last_frame_; }
	bool hasLastFrame() const { return has_last_frame_; }

private:
	void processEvents();
	void update(float dt);
	void render();
	void applyPendingState();
	std::unique_ptr<State> createState(StateId id);

	void loadHighScores();
	void saveHighScores() const;

	sf::RenderWindow window_;
	ResourceManager<sf::Texture> textures_;
	ResourceManager<sf::Font> fonts_;
	AudioManager audio_;
	Settings settings_;

	std::unique_ptr<State> current_state_;
	StateId pending_state_ = StateId::Menu;
	bool has_pending_state_ = false;

	// One stored best score per difficulty (Easy / Medium / Hard).
	std::array<int, difficulty::COUNT> high_scores_{};
	int last_score_ = 0;

	// Snapshot of the final gameplay frame, shown on the game-over screen.
	sf::Texture last_frame_;
	bool has_last_frame_ = false;
};
