// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include <SFML/Graphics/Color.hpp>
#include <string>

// ---------------------------------------------------------------------------
//  Constants.hpp
//  A single, central place for every tunable value in the game.
//  The project rubric forbids "magic numbers": every literal that controls
//  gameplay, layout or physics lives here inside the `cfg` namespace so it can
//  be reused, documented and tweaked without hunting through the code base.
// ---------------------------------------------------------------------------
namespace cfg {

// ------------------------------- Window ------------------------------------
namespace window {
inline constexpr unsigned int WIDTH = 480u;
inline constexpr unsigned int HEIGHT = 760u;
inline constexpr unsigned int FRAMERATE_LIMIT = 120u;
inline constexpr char TITLE[] = "Simple Doodle Jump";
// Physics may never advance by more than this in a single frame. This keeps
// the simulation stable if the machine stalls (prevents "tunnelling").
inline constexpr float MAX_DELTA_TIME = 1.0f / 30.0f;
} // namespace window

// ------------------------------- Assets ------------------------------------
namespace assets {
inline const std::string BACKGROUND = "assets/background.png";
inline const std::string LEFT_DOODLE = "assets/left_doodle.png";
inline const std::string RIGHT_DOODLE = "assets/right_doodle.png";
inline const std::string NORMAL_PLATFORM = "assets/normal_platform.png";
inline const std::string MOVING_PLATFORM = "assets/moving_platform.png";
inline const std::string BROKEN_PLATFORM = "assets/broken_platform.png";
inline const std::string SPRING = "assets/spring_sprite.png";
inline const std::string START_BUTTON = "assets/start_button.png";
inline const std::string RESTART_BUTTON = "assets/restart_button.png";
inline const std::string MENU_BUTTON = "assets/menu_button.png";
inline const std::string FONT = "fonts/ariblk.ttf";
} // namespace assets

// ------------------------------- Player ------------------------------------
namespace player {
inline constexpr float SCALE = 0.6f;          // doodle sprites are quite large
inline constexpr float MOVE_SPEED = 340.0f;    // horizontal speed  (px / second)
// One score point per this many pixels climbed.
inline constexpr float PIXELS_PER_POINT = 10.0f;
} // namespace player

// ------------------------------- Physics -----------------------------------
namespace physics {
inline constexpr float GRAVITY = 1800.0f;        // downward acceleration px/s^2
inline constexpr float NORMAL_JUMP = -900.0f;     // impulse from a normal jump
inline constexpr float SPRING_JUMP = -1650.0f;    // impulse from a spring
// Maximum reachable height for a normal jump:  v^2 / (2g)  ~= 225 px.
// Every vertical gap below stays well under this so the world is always
// beatable (see world::MAX_GAP).
} // namespace physics

// ------------------------------ Platforms ----------------------------------
namespace platform {
inline constexpr float MOVING_SPEED = 90.0f;      // horizontal speed of movers
inline constexpr float BREAKABLE_FALL_SPEED = 450.0f; // fall speed once broken
inline constexpr float SPRING_CHANCE = 0.18f;     // chance a normal platform
                                                  // carries a spring
// Weighted random selection of the platform that forms the next safe rung.
inline constexpr int WEIGHT_NORMAL = 60;
inline constexpr int WEIGHT_MOVING = 25;
inline constexpr int WEIGHT_BREAKABLE = 15;
} // namespace platform

// -------------------------------- World ------------------------------------
namespace world {
// Vertical distance between consecutive platforms.
// MAX_GAP * 2 (= 190) stays below the ~225 px max jump height, which
// guarantees the player can always skip a single (e.g. breakable) platform.
inline constexpr float MIN_GAP = 55.0f;
inline constexpr float MAX_GAP = 95.0f;
// Generate platforms this far above the visible top so they are ready in time.
inline constexpr float GENERATION_BUFFER = 120.0f;
// A small tolerance used when deciding a platform has scrolled off-screen.
inline constexpr float REMOVAL_BUFFER = 40.0f;
} // namespace world

// -------------------------------- UI ---------------------------------------
namespace ui {
inline constexpr float BUTTON_SCALE = 0.55f;
inline constexpr unsigned int TITLE_TEXT_SIZE = 54u;
inline constexpr unsigned int SCORE_TEXT_SIZE = 30u;
inline constexpr unsigned int LABEL_TEXT_SIZE = 28u;
inline constexpr unsigned int GAME_OVER_TEXT_SIZE = 60u;
inline const sf::Color TEXT_COLOR = sf::Color(40, 40, 60);
inline const sf::Color ACCENT_COLOR = sf::Color(224, 90, 60);
inline const sf::Color BACKGROUND_CLEAR = sf::Color(247, 244, 235);
} // namespace ui

} // namespace cfg
