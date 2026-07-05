// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "world/CollisionSystem.hpp"

#include "entities/Player.hpp"
#include "entities/platforms/Platform.hpp"

#include <SFML/Graphics/Rect.hpp>

namespace {
// A small forgiveness band (in pixels) around a platform's top edge.
constexpr float LANDING_TOLERANCE = 6.0f;
} // namespace

bool CollisionSystem::horizontallyOverlaps(const Player& player,
                                           const Platform& platform) {
	const sf::FloatRect p = player.getBounds();
	const sf::FloatRect f = platform.getBounds();
	return p.left < f.left + f.width && p.left + p.width > f.left;
}

void CollisionSystem::resolve(Player& player, std::vector<Platform*>& platforms,
                              float dt) const {
	const float vy = player.getVerticalVelocity();

	// Only downward motion can land on / interact with a platform top.
	if (vy <= 0.0f) {
		return;
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
				}
				// Landing resolved for this frame -> stop checking.
				return;
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
				return;
			}
		}
	}
}
