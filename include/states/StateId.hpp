// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

// The top-level game states, matching the spec's state diagram:
//   Menu -> Play -> GameOver -> (Play again | back to Menu)
//   Menu -> Settings -> Menu   (settings are only reachable from the menu)
enum class StateId {
	Menu,
	Settings,
	Play,
	GameOver,
};
