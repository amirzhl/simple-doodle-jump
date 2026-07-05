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
#include "states/State.hpp"

#include <SFML/System/Clock.hpp>
#include <algorithm>
#include <fstream>

namespace {
const char* HIGH_SCORE_FILE = "highscore.txt";
} // namespace

Game::Game()
	: window_(sf::VideoMode(cfg::window::WIDTH, cfg::window::HEIGHT),
	          cfg::window::TITLE, sf::Style::Titlebar | sf::Style::Close) {
	window_.setFramerateLimit(cfg::window::FRAMERATE_LIMIT);
	loadHighScore();
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
		case StateId::Play:
			return std::make_unique<PlayState>(*this);
		case StateId::GameOver:
			return std::make_unique<GameOverState>(*this);
	}
	return std::make_unique<MenuState>(*this);
}

void Game::submitScore(int score) {
	last_score_ = score;
	if (score > high_score_) {
		high_score_ = score;
		saveHighScore();
	}
}

void Game::loadHighScore() {
	std::ifstream in(HIGH_SCORE_FILE);
	if (in) {
		int value = 0;
		if (in >> value && value >= 0) {
			high_score_ = value;
		}
	}
}

void Game::saveHighScore() const {
	std::ofstream out(HIGH_SCORE_FILE, std::ios::trunc);
	if (out) {
		out << high_score_ << '\n';
	}
}
