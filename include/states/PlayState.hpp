// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include "entities/Player.hpp"
#include "states/State.hpp"
#include "world/CollisionSystem.hpp"
#include "world/PlatformManager.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/View.hpp>
#include <memory>

// ---------------------------------------------------------------------------
//  PlayState  -- the actual gameplay. It wires together the player, the
//  platform manager and the collision system, drives the camera (view) that
//  scrolls the endless world, draws the live score, and detects the fall that
//  ends the game.
//
//  Camera strategy (per the TA hint): the player really moves upward in world
//  space; a vertical-only sf::View follows it up but never back down, so the
//  player appears fixed near the centre while the world scrolls beneath.
// ---------------------------------------------------------------------------
class PlayState : public State {
public:
	explicit PlayState(Game& game);

	void handleEvent(const sf::Event& event) override;
	void update(float dt) override;
	void render(sf::RenderWindow& window) override;

private:
	void updateCamera();
	bool playerHasFallen() const;
	void drawHud(sf::RenderWindow& window) const;

	sf::Sprite background_;
	std::unique_ptr<Player> player_;
	PlatformManager platforms_;
	CollisionSystem collision_;

	sf::View world_view_;   // scrolls with the player
	sf::Text score_text_;
};
