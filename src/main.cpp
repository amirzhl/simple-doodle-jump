// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "core/Game.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

// ---------------------------------------------------------------------------
//  Entry point. Deliberately tiny: all logic lives inside classes (OOP
//  requirement). main() only constructs the Game and runs it, and reports any
//  fatal error (e.g. a missing asset) cleanly.
// ---------------------------------------------------------------------------
int main() {
	try {
		Game game;
		game.run();
	} catch (const std::exception& error) {
		std::cerr << "Fatal error: " << error.what() << '\n';
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
