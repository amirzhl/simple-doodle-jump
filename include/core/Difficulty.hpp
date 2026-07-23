// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

// ---------------------------------------------------------------------------
//  Difficulty  -- the three selectable difficulty levels. Kept as a strongly
//  typed enum so the compiler catches invalid values, with tiny free helpers
//  to convert to/from the on-disk integer index and to a display name.
//
//  Effects (implemented across World / Bullet / PlayState):
//    Easy   : weaker monsters, faster fire rate, NO holes
//    Medium : hardier monsters, faster moving platforms, NO holes
//    Hard   : like Medium, plus holes are enabled
// ---------------------------------------------------------------------------
enum class Difficulty {
	Easy,
	Medium,
	Hard,
};

namespace difficulty {

inline constexpr int COUNT = 3;

inline int toIndex(Difficulty level) {
	return static_cast<int>(level);
}

inline Difficulty fromIndex(int index) {
	if (index <= 0) {
		return Difficulty::Easy;
	}
	if (index >= COUNT - 1) {
		return Difficulty::Hard;
	}
	return Difficulty::Medium;
}

inline const char* name(Difficulty level) {
	switch (level) {
		case Difficulty::Easy:
			return "Easy";
		case Difficulty::Medium:
			return "Medium";
		case Difficulty::Hard:
			return "Hard";
	}
	return "Medium";
}

// Holes are only active on Hard.
inline bool holesEnabled(Difficulty level) {
	return level == Difficulty::Hard;
}

} // namespace difficulty
