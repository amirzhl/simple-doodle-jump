// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "utils/Random.hpp"

#include <random>

namespace {
// One engine for the whole program, seeded once from a hardware entropy
// source. Kept in an anonymous namespace so it is private to this file.
std::mt19937& engine() {
	static std::mt19937 gen(std::random_device{}());
	return gen;
}
} // namespace

namespace Random {

int intInRange(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(engine());
}

float floatInRange(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(engine());
}

bool chance(float probability) {
	std::bernoulli_distribution dist(probability);
	return dist(engine());
}

int weightedChoice(int weight0, int weight1, int weight2) {
	const int total = weight0 + weight1 + weight2;
	if (total <= 0) {
		return 0;
	}
	std::uniform_int_distribution<int> dist(0, total - 1);
	const int roll = dist(engine());
	if (roll < weight0) {
		return 0;
	}
	if (roll < weight0 + weight1) {
		return 1;
	}
	return 2;
}

} // namespace Random
