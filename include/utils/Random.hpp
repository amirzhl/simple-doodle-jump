// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

// ---------------------------------------------------------------------------
//  Random  -- a tiny wrapper around <random> so the whole project shares one
//  well-seeded Mersenne-Twister engine. Centralising randomness keeps the
//  generators clean and makes the behaviour easy to reason about.
// ---------------------------------------------------------------------------
namespace Random {

// Uniform integer in the inclusive range [min, max].
int intInRange(int min, int max);

// Uniform float in the range [min, max).
float floatInRange(float min, float max);

// Returns true with the given probability (0.0 .. 1.0).
bool chance(float probability);

// Weighted choice between three options; returns 0, 1 or 2 with probability
// proportional to the supplied weights.
int weightedChoice(int weight0, int weight1, int weight2);

} // namespace Random
