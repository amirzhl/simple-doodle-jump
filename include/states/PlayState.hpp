// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include "core/Difficulty.hpp"
#include "entities/Player.hpp"
#include "states/State.hpp"
#include "world/CollisionSystem.hpp"
#include "world/World.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <memory>
#include <vector>

namespace sf {
class RenderTarget;
}
class Bullet;

// ---------------------------------------------------------------------------
//  PlayState  -- the actual gameplay. It wires together the player, the World
//  (platforms + monsters + holes), the collision system and the bullets, and
//  drives the camera (view) that scrolls the endless world.
//
//  Camera strategy (per the TA hint): the player really moves upward in world
//  space; a vertical-only sf::View follows it up but never back down, so the
//  player appears fixed near the centre while the world scrolls beneath.
//
//  Phase 2 additions handled here:
//    * shooting: holding Space fires bullets up on a difficulty-based cadence
//    * monsters: head-bounce survives, any other touch ends the game, bullets
//      damage/kill them
//    * holes: touching one triggers a short "suck the player in" animation
//      that freezes the world and then ends the game
//    * audio: jump / shoot / lose sound effects
// ---------------------------------------------------------------------------
class PlayState : public State {
public:
	explicit PlayState(Game& game);
	~PlayState();

	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;

private:
	void updateBullets(float dt, float world_top);
	void handleShooting(float dt, bool firing);
	void resolveBulletMonsterHits();
	float fireInterval() const;

	Hole* holeUnderPlayer();
	void beginSuction(sf::Vector2f hole_center);
	void updateSuction(float dt);

	void endGame();
	void captureDeathFrame();
	void updateCamera();
	bool playerHasFallen() const;
	void clearBullets();
	void drawScene(sf::RenderTarget& target);
	void drawHud(sf::RenderTarget& target) const;

	sf::Sprite background_;
	Difficulty difficulty_;
	World world_;
	CollisionSystem collision_;
	std::unique_ptr<Player> player_;
	std::vector<Bullet*> bullets_;

	sf::View world_view_;   // scrolls with the player
	sf::Text score_text_;

	float fire_timer_ = 0.0f;

	// Hole suction animation state.
	bool sucking_ = false;
	float suck_progress_ = 0.0f;
	sf::Vector2f suck_start_center_;
	sf::Vector2f hole_center_;
};
