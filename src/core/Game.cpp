// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "core/Game.hpp"

#include "core/Constants.hpp"
#include "states/GameOverState.hpp"
#include "states/MenuState.hpp"
#include "states/PlayState.hpp"
#include "states/SettingsState.hpp"
#include "states/State.hpp"

#include <SFML/System/Clock.hpp>
#include <algorithm>
#include <fstream>

namespace {
const char* HIGH_SCORE_FILE = "highscore.txt";
const char* SETTINGS_FILE = "settings.txt";
} // namespace

Game::Game()
	: window_(sf::VideoMode(cfg::window::WIDTH, cfg::window::HEIGHT),
	          cfg::window::TITLE, sf::Style::Titlebar | sf::Style::Close) {
	window_.setFramerateLimit(cfg::window::FRAMERATE_LIMIT);

	// Load persisted settings first, then apply the saved volume to all audio.
	settings_.load(SETTINGS_FILE);
	audio_.setVolume(settings_.volume());

	loadHighScores();
	current_state_ = createState(StateId::Menu);
}

// Defined here (not =default in the header) so State is complete at the point
// the unique_ptr<State> destructor is instantiated.
Game::~Game() = default;

void Game::run() {
	sf::Clock clock;
	while (window_.isOpen()) {
		float dt = clock.restart().asSeconds();
		// Clamp dt so a stall can never make the physics explode / tunnel.
		dt = std::min(dt, cfg::window::MAX_DELTA_TIME);

		processEvents();
		update(dt);
		render();
		applyPendingState();
	}
}

void Game::processEvents() {
	sf::Event event;
	while (window_.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window_.close();
		}
		if (current_state_) {
			current_state_->handleEvent(event);
		}
	}
}

void Game::update(float dt) {
	if (current_state_) {
		current_state_->update(dt);
	}
}

void Game::render() {
	window_.clear(cfg::ui::BACKGROUND_CLEAR);
	if (current_state_) {
		current_state_->render(window_);
	}
	window_.display();
}

void Game::requestState(StateId id) {
	pending_state_ = id;
	has_pending_state_ = true;
}

void Game::applyPendingState() {
	if (!has_pending_state_) {
		return;
	}
	has_pending_state_ = false;
	current_state_ = createState(pending_state_);
}

std::unique_ptr<State> Game::createState(StateId id) {
	switch (id) {
		case StateId::Menu:
			return std::make_unique<MenuState>(*this);
		case StateId::Settings:
			return std::make_unique<SettingsState>(*this);
		case StateId::Play:
			return std::make_unique<PlayState>(*this);
		case StateId::GameOver:
			return std::make_unique<GameOverState>(*this);
	}
	return std::make_unique<MenuState>(*this);
}

int Game::highScore() const {
	return high_scores_[static_cast<std::size_t>(
		difficulty::toIndex(settings_.difficulty()))];
}

void Game::submitScore(int score) {
	last_score_ = score;
	const std::size_t index =
		static_cast<std::size_t>(difficulty::toIndex(settings_.difficulty()));
	if (score > high_scores_[index]) {
		high_scores_[index] = score;
		saveHighScores();
	}
}

void Game::saveSettings() const {
	settings_.save(SETTINGS_FILE);
}

void Game::loadHighScores() {
	std::ifstream in(HIGH_SCORE_FILE);
	if (!in) {
		return;
	}
	// One line per difficulty; tolerate a short/old file by keeping defaults.
	for (std::size_t i = 0; i < high_scores_.size(); ++i) {
		int value = 0;
		if (in >> value && value >= 0) {
			high_scores_[i] = value;
		} else {
			break;
		}
	}
}

void Game::saveHighScores() const {
	std::ofstream out(HIGH_SCORE_FILE, std::ios::trunc);
	if (out) {
		for (int value : high_scores_) {
			out << value << '\n';
		}
	}
}
