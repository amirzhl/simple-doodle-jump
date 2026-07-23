// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "world/CollisionSystem.hpp"

#include "core/Constants.hpp"
#include "entities/Monster.hpp"
#include "entities/Player.hpp"
#include "entities/platforms/Platform.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace {
// A small forgiveness band (in pixels) around a top edge.
constexpr float LANDING_TOLERANCE = 6.0f;

bool horizontalOverlap(const sf::FloatRect& a, const sf::FloatRect& b) {
	return a.left < b.left + b.width && a.left + a.width > b.left;
}

bool verticalOverlap(const sf::FloatRect& a, const sf::FloatRect& b) {
	return a.top < b.top + b.height && a.top + a.height > b.top;
}
} // namespace

bool CollisionSystem::horizontallyOverlaps(const Player& player,
                                           const Platform& platform) {
	return horizontalOverlap(player.getBounds(), platform.getBounds());
}

bool CollisionSystem::resolve(Player& player, std::vector<Platform*>& platforms,
                              float dt) const {
	const float vy = player.getVerticalVelocity();

	// Only downward motion can land on / interact with a platform top.
	if (vy <= 0.0f) {
		return false;
	}

	const sf::FloatRect pb = player.getBounds();
	const float feet = pb.top + pb.height;
	const float prev_feet = feet - vy * dt; // where the feet were last frame

	for (Platform* platform : platforms) {
		if (!horizontallyOverlaps(player, *platform)) {
			continue;
		}

		const sf::FloatRect fb = platform->getBounds();
		const float top = fb.top;

		if (platform->isSupportive()) {
			// Swept test: feet crossed the top surface during this frame.
			const bool crossed_top =
				prev_feet <= top + LANDING_TOLERANCE && feet >= top;
			if (crossed_top) {
				player.setFeetY(top);
				const float impulse = platform->onLand(player);
				if (impulse != 0.0f) {
					player.jump(impulse);
					return true; // a jump happened -> caller plays the sound
				}
				return false;
			}
		} else {
			// A broken (falling) breakable platform: if the player is resting on
			// it, drag the player down with it. The player keeps horizontal
			// control, so steering onto a healthy platform is a rescue chance.
			const bool resting = feet >= top - LANDING_TOLERANCE &&
			                     feet <= top + fb.height + LANDING_TOLERANCE;
			if (resting) {
				player.setFeetY(top);
				player.setVerticalVelocity(platform->supportVelocity());
				return false;
			}
		}
	}
	return false;
}

CollisionSystem::MonsterHit CollisionSystem::resolveMonsters(
	Player& player, std::vector<Monster*>& monsters, float dt) const {
	const float vy = player.getVerticalVelocity();
	const sf::FloatRect pb = player.getBounds();
	const float feet = pb.top + pb.height;
	const float prev_feet = feet - vy * dt;

	for (Monster* monster : monsters) {
		if (!monster->isAlive()) {
			continue;
		}
		const sf::FloatRect mb = monster->getBounds();
		if (!horizontalOverlap(pb, mb)) {
			continue;
		}

		const float top = mb.top;
		const float mid = mb.top + mb.height * 0.5f;

		// "Coming from above" ALWAYS makes the player bounce and survive, exactly
		// like landing on a platform (it is never fatal). We treat it as a head-
		// bounce when the player is falling AND its feet reach the monster, AND
		// either its feet crossed the monster's top this frame (the normal swept
		// test) OR its feet are still within the monster's upper half. The second
		// clause keeps the stomp forgiving during very fast descents, so a genuine
		// head-landing can never be mis-classified as a fatal side hit.
		const bool falling = vy > 0.0f;
		const bool from_above =
			falling && feet >= top &&
			(prev_feet <= top + LANDING_TOLERANCE || feet <= mid);
		if (from_above) {
			player.setFeetY(top);
			player.jump(cfg::monster::BOUNCE_JUMP);
			return MonsterHit::Bounced;
		}

		// Otherwise a real overlap (hit from the side or from below, or rising
		// up into the monster) is fatal.
		if (verticalOverlap(pb, mb)) {
			return MonsterHit::Killed;
		}
	}
	return MonsterHit::None;
}
