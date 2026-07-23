// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include "entities/Spring.hpp"
#include "entities/platforms/Platform.hpp"

#include <memory>

// ---------------------------------------------------------------------------
//  NormalPlatform  -- a stable, standard platform. It is the only platform
//  type that may carry a Spring power-up (owned via unique_ptr, so no manual
//  delete is ever required and there are no leaks).
//
//  When the player lands on it:
//    * with a spring  -> the spring compresses and a very strong jump is given
//    * without spring -> a standard jump
// ---------------------------------------------------------------------------
class NormalPlatform : public Platform {
public:
	NormalPlatform(const sf::Texture& texture, sf::Vector2f position);

	// Optionally attach a spring, positioned on top of this platform.
	void attachSpring(const sf::Texture& spring_texture);

	void update(float dt) override;
	void draw(sf::RenderTarget& target) const override;

	float onLand(Player& player) override;

	bool hasSpring() const { return spring_ != nullptr; }

	// The attached spring (nullptr when this platform has none).
	const Spring* spring() const { return spring_.get(); }

private:
	std::unique_ptr<Spring> spring_;
};
