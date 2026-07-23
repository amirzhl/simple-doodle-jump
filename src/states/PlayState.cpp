// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "states/PlayState.hpp"

#include "audio/AudioManager.hpp"
#include "core/Constants.hpp"
#include "core/Game.hpp"
#include "core/Settings.hpp"
#include "entities/Bullet.hpp"
#include "entities/Hole.hpp"
#include "entities/Monster.hpp"
#include "entities/platforms/Platform.hpp"
#include "ui/Layout.hpp"

#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>

PlayState::PlayState(Game& game)
	: State(game), difficulty_(game.settings().difficulty()),
	  world_(game.textures(), game.settings().difficulty()) {
	const float screen_width = static_cast<float>(cfg::window::WIDTH);
	const float screen_height = static_cast<float>(cfg::window::HEIGHT);

	// Gameplay has no background music -- silence anything left from the menu.
	game_.audio().stopMusic();

	// Background is drawn in the fixed HUD view so it never scrolls.
	ui::stretchToWindow(background_,
	                    game_.textures().acquire(cfg::assets::BACKGROUND));

	// Build the world, then place the player on top of the starting platform.
	world_.reset();
	player_ = std::make_unique<Player>(
		game_.textures().acquire(cfg::assets::LEFT_DOODLE),
		game_.textures().acquire(cfg::assets::RIGHT_DOODLE),
		game_.textures().acquire(cfg::assets::SHOOTING_POSE));
	const sf::FloatRect player_bounds = player_->getBounds();
	player_->reset(sf::Vector2f(
		screen_width / 2.0f - player_bounds.width / 2.0f,
		world_.startingPlatformTop() - player_bounds.height));
	// Give the very first automatic jump so the player launches immediately.
	player_->jump(cfg::physics::NORMAL_JUMP);

	world_view_.setSize(screen_width, screen_height);
	world_view_.setCenter(screen_width / 2.0f, screen_height / 2.0f);

	const sf::Font& font = game_.fonts().acquire(cfg::assets::FONT);
	score_text_.setFont(font);
	score_text_.setCharacterSize(cfg::ui::SCORE_TEXT_SIZE);
	score_text_.setFillColor(cfg::ui::ACCENT_COLOR); // stands out over the scene
	score_text_.setPosition(14.0f, 10.0f);
}

PlayState::~PlayState() {
	clearBullets();
}

void PlayState::clearBullets() {
	for (Bullet* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();
}

void PlayState::handleEvent(const sf::Event& /*event*/) {
	// Continuous movement / firing are polled directly in update().
}

float PlayState::fireInterval() const {
	return difficulty_ == Difficulty::Easy ? cfg::bullet::FIRE_INTERVAL_EASY
	                                       : cfg::bullet::FIRE_INTERVAL_HARD;
}

void PlayState::update(float dt) {
	// While the player is being sucked into a hole the whole world freezes and
	// only the short animation plays out.
	if (sucking_) {
		updateSuction(dt);
		return;
	}

	player_->handleInput();

	// Holding Space keeps the shooting pose on and fires on a cadence.
	const bool firing = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	if (firing) {
		player_->startShooting();
	} else {
		player_->stopShooting();
	}

	player_->update(dt);

	// The camera window in world coordinates, derived from the scrolling view.
	const float world_top = world_view_.getCenter().y -
	                        static_cast<float>(cfg::window::HEIGHT) / 2.0f;
	const float world_bottom = world_view_.getCenter().y +
	                           static_cast<float>(cfg::window::HEIGHT) / 2.0f;
	world_.update(dt, world_top, world_bottom);

	updateBullets(dt, world_top);
	handleShooting(dt, firing);

	// Holes take priority: if the player touches one, begin the suck-in ending.
	if (Hole* hole = holeUnderPlayer()) {
		beginSuction(hole->center());
		return;
	}

	// Monsters: a head-bounce survives; any other contact is fatal.
	const CollisionSystem::MonsterHit hit =
		collision_.resolveMonsters(*player_, world_.monsters(), dt);
	if (hit == CollisionSystem::MonsterHit::Killed) {
		endGame();
		return;
	}
	if (hit == CollisionSystem::MonsterHit::Bounced) {
		game_.audio().playJump();
	}

	// Platforms/springs: the same detection as phase 1, now reporting jumps so
	// we can play the jump sound.
	if (collision_.resolve(*player_, world_.platforms(), dt)) {
		game_.audio().playJump();
	}

	resolveBulletMonsterHits();

	updateCamera();
	score_text_.setString("Score: " + std::to_string(player_->getScore()));

	if (playerHasFallen()) {
		endGame();
	}
}

void PlayState::updateBullets(float dt, float world_top) {
	std::vector<Bullet*> survivors;
	survivors.reserve(bullets_.size());
	for (Bullet* bullet : bullets_) {
		bullet->update(dt);
		if (bullet->isOffscreen(world_top)) {
			delete bullet; // left the top of the view -> free it (no leaks)
		} else {
			survivors.push_back(bullet);
		}
	}
	bullets_.swap(survivors);
}

void PlayState::handleShooting(float dt, bool firing) {
	if (!firing) {
		fire_timer_ = 0.0f; // fire immediately on the next press
		return;
	}
	fire_timer_ -= dt;
	if (fire_timer_ <= 0.0f) {
		fire_timer_ = fireInterval();
		bullets_.push_back(new Bullet(
			game_.textures().acquire(cfg::assets::NOSE),
			player_->getMuzzlePosition()));
		game_.audio().playShoot();
	}
}

void PlayState::resolveBulletMonsterHits() {
	std::vector<Monster*>& monsters = world_.monsters();
	std::vector<Bullet*> surviving_bullets;
	surviving_bullets.reserve(bullets_.size());

	for (Bullet* bullet : bullets_) {
		bool consumed = false;
		for (Monster* monster : monsters) {
			if (monster->isAlive() &&
			    bullet->getBounds().intersects(monster->getBounds())) {
				monster->takeDamage(1); // World frees it once health hits 0
				consumed = true;
				break;
			}
		}
		if (consumed) {
			delete bullet;
		} else {
			surviving_bullets.push_back(bullet);
		}
	}
	bullets_.swap(surviving_bullets);
}

Hole* PlayState::holeUnderPlayer() {
	const sf::FloatRect pb = player_->getBounds();
	for (Hole* hole : world_.holes()) {
		if (pb.intersects(hole->getBounds())) {
			return hole;
		}
	}
	return nullptr;
}

void PlayState::beginSuction(sf::Vector2f hole_center) {
	// Drop the shooting pose so the shrinking doodle animates correctly.
	player_->stopShooting();
	sucking_ = true;
	suck_progress_ = 0.0f;
	hole_center_ = hole_center;
	const sf::FloatRect pb = player_->getBounds();
	suck_start_center_ =
		sf::Vector2f(pb.left + pb.width / 2.0f, pb.top + pb.height / 2.0f);
}

void PlayState::updateSuction(float dt) {
	suck_progress_ += dt / cfg::hole::SUCK_DURATION;
	if (suck_progress_ >= 1.0f) {
		endGame();
		return;
	}
	// Lerp the player's centre toward the hole while shrinking it.
	const float t = suck_progress_;
	const sf::Vector2f center(
		suck_start_center_.x + (hole_center_.x - suck_start_center_.x) * t,
		suck_start_center_.y + (hole_center_.y - suck_start_center_.y) * t);
	player_->setVisualScale(cfg::player::SCALE * (1.0f - t));
	player_->setCenter(center);
}

void PlayState::endGame() {
	game_.audio().playLose();
	captureDeathFrame();
	game_.submitScore(player_->getScore());
	game_.requestState(StateId::GameOver);
}

void PlayState::captureDeathFrame() {
	// Render the current scene into an off-screen texture and hand it to the
	// Game, so the game-over screen can display the frozen "where you lost"
	// moment behind its frosted overlay.
	sf::RenderTexture frame;
	if (!frame.create(cfg::window::WIDTH, cfg::window::HEIGHT)) {
		return;
	}
	frame.clear(cfg::ui::BACKGROUND_CLEAR);
	drawScene(frame);
	frame.display();
	game_.setLastFrame(frame.getTexture());
}

void PlayState::updateCamera() {
	// Follow the player upward only: the view centre never descends again.
	const sf::FloatRect bounds = player_->getBounds();
	const float player_center_y = bounds.top + bounds.height / 2.0f;
	if (player_center_y < world_view_.getCenter().y) {
		world_view_.setCenter(world_view_.getCenter().x, player_center_y);
	}
}

bool PlayState::playerHasFallen() const {
	const float view_bottom = world_view_.getCenter().y +
	                          static_cast<float>(cfg::window::HEIGHT) / 2.0f;
	return player_->getBounds().top > view_bottom;
}

void PlayState::render(sf::RenderWindow& window) {
	drawScene(window);
}

// Draw the full gameplay scene (background + scrolling world + HUD) onto ANY
// render target. Shared by the live window and by the death-frame capture so
// the game-over screen can freeze exactly what was on screen when you lost.
void PlayState::drawScene(sf::RenderTarget& target) {
	// 1) Fixed background in the default view.
	target.setView(target.getDefaultView());
	target.draw(background_);

	// 2) The scrolling world: holes, platforms, monsters, bullets, player.
	target.setView(world_view_);
	for (const Hole* hole : world_.holes()) {
		hole->draw(target);
	}
	for (const Platform* platform : world_.platforms()) {
		platform->draw(target);
	}
	for (const Monster* monster : world_.monsters()) {
		monster->draw(target);
	}
	for (const Bullet* bullet : bullets_) {
		bullet->draw(target);
	}
	player_->draw(target);

	// 3) The HUD (score) back in the fixed view.
	drawHud(target);
}

void PlayState::drawHud(sf::RenderTarget& target) const {
	target.setView(target.getDefaultView());
	target.draw(score_text_);
}
