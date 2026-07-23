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
//
//  Phase 2 additions (monsters, shooting, sound, settings, holes) extend this
//  same file rather than scattering new literals across the new classes.
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
inline const std::string SHOOTING_POSE = "assets/Shooting@Pose.png";
inline const std::string NOSE = "assets/Nose.png"; // projectile (bullet) sprite
inline const std::string NORMAL_PLATFORM = "assets/normal_platform.png";
inline const std::string MOVING_PLATFORM = "assets/moving_platform.png";
inline const std::string BROKEN_PLATFORM = "assets/broken_platform.png";
inline const std::string SPRING = "assets/spring_sprite.png";
inline const std::string BLUE_MONSTER = "assets/BlueMonster.png";
inline const std::string GREEN_MONSTER = "assets/green_monster.png";
inline const std::string HOLE_SMALL = "assets/hole.png";
inline const std::string HOLE_LARGE = "assets/hole@2x.png";
inline const std::string START_BUTTON = "assets/start_button.png";
inline const std::string RESTART_BUTTON = "assets/restart_button.png";
inline const std::string MENU_BUTTON = "assets/menu_button.png";
inline const std::string SETTINGS_BUTTON = "assets/Settings_button.png";
inline const std::string BACK_BUTTON = "assets/back_button.png";
inline const std::string FONT = "fonts/ariblk.ttf";
} // namespace assets

// ------------------------------- Sounds ------------------------------------
namespace sound {
inline const std::string MENU_MUSIC = "sounds/MainMenu_Song.flac";
inline const std::string SFX_JUMP = "sounds/Jumping_Sound.wav";
inline const std::string SFX_SHOOT = "sounds/Shooting_Sound.wav";
inline const std::string SFX_LOSE = "sounds/Loosing_Sound.wav";
} // namespace sound

// ------------------------------- Audio -------------------------------------
namespace audio {
inline constexpr int MIN_VOLUME = 0;
inline constexpr int MAX_VOLUME = 100;
inline constexpr int DEFAULT_VOLUME = 60;
// Number of concurrent sf::Sound voices for overlapping effects (e.g. rapid
// shooting). One buffer is shared per effect via the ResourceManager cache.
inline constexpr int SOUND_POOL_SIZE = 16;
} // namespace audio

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
inline constexpr float MOVING_SPEED = 90.0f;      // base horizontal speed
inline constexpr float BREAKABLE_FALL_SPEED = 450.0f; // fall speed once broken
inline constexpr float SPRING_CHANCE = 0.18f;     // chance a normal platform
                                                  // carries a spring
// Weighted random selection of the platform that forms the next safe rung.
inline constexpr int WEIGHT_NORMAL = 60;
inline constexpr int WEIGHT_MOVING = 25;
inline constexpr int WEIGHT_BREAKABLE = 15;
} // namespace platform

// ------------------------------- Monsters ----------------------------------
namespace monster {
inline constexpr float SPEED = 70.0f;             // horizontal patrol speed
inline constexpr float SCALE = 0.7f;              // sprite scale (fit the world)
// A landing on the monster's head throws the player up strongly.
inline constexpr float BOUNCE_JUMP = -1200.0f;
// Initial health scales with difficulty (tougher levels -> hardier monsters).
inline constexpr int HEALTH_EASY = 1;
inline constexpr int HEALTH_MEDIUM = 3;
inline constexpr int HEALTH_HARD = 4;
// A monster patrols only this far to each side of its spawn point, so it
// never sweeps the whole screen and never blocks the only route upward.
inline constexpr float PATROL_RANGE = 90.0f;
} // namespace monster

// -------------------------------- Bullets ----------------------------------
namespace bullet {
inline constexpr float SPEED = 720.0f;            // upward speed (magnitude)
inline constexpr float SCALE = 0.6f;              // projectile sprite scale
// Cadence between shots; Easy shoots faster than Medium/Hard.
inline constexpr float FIRE_INTERVAL_EASY = 0.25f;
inline constexpr float FIRE_INTERVAL_HARD = 0.5f; // used by Medium & Hard
inline constexpr float OFFSCREEN_BUFFER = 40.0f;
// Visible part of assets/Nose.png (the rest of the canvas is transparent).
inline constexpr int ART_LEFT = 1;
inline constexpr int ART_TOP = 0;
inline constexpr int ART_WIDTH = 24;
inline constexpr int ART_HEIGHT = 38;
} // namespace bullet

// --------------------------------- Holes -----------------------------------
namespace hole {
// Fixed, short duration of the "suck" animation before the game ends.
inline constexpr float SUCK_DURATION = 0.9f;
} // namespace hole

// ------------------------- Hazard generation -------------------------------
//  Shared rules for spawning the new side-hazards (monsters and holes).
namespace hazard {
inline constexpr float START_DELAY = 3.0f;         // no hazards at game start
inline constexpr float MIN_SPAWN_INTERVAL = 1.4f;  // random spawn cadence
inline constexpr float MAX_SPAWN_INTERVAL = 2.8f;
inline constexpr float SIDE_GAP = 14.0f;           // gap platform <-> hazard
inline constexpr float OVERLAP_PADDING = 6.0f;     // min spacing between objects
inline constexpr float HOLE_SHARE = 0.45f;         // Hard: share of hazards that
                                                   // are holes (rest: monsters)
// Moving-platform speed multiplier per difficulty (Medium/Hard are faster).
inline constexpr float MOVING_SPEED_MULT_EASY = 1.0f;
inline constexpr float MOVING_SPEED_MULT_HARD = 1.6f;
} // namespace hazard

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
// Every button is rendered with this on-screen width. The source images
// come in mixed resolutions (400x100 and 800x200), so a fixed scale factor
// would make some buttons twice as large as others.
inline constexpr float BUTTON_TARGET_WIDTH = 220.0f;
inline constexpr unsigned int TITLE_TEXT_SIZE = 54u;
inline constexpr unsigned int SCORE_TEXT_SIZE = 30u;
inline constexpr unsigned int LABEL_TEXT_SIZE = 28u;
inline constexpr unsigned int GAME_OVER_TEXT_SIZE = 60u;
inline const sf::Color TEXT_COLOR = sf::Color(40, 40, 60);
inline const sf::Color ACCENT_COLOR = sf::Color(224, 90, 60);
inline const sf::Color MUTED_COLOR = sf::Color(150, 150, 160);
inline const sf::Color BACKGROUND_CLEAR = sf::Color(247, 244, 235);
// Frosted veil drawn over the frozen "where you lost" snapshot on the game-
// over screen: a light, semi-transparent overlay = the "matte" look the TA
// asked for (the death scene stays visible but dimmed underneath).
inline const sf::Color GAMEOVER_MATTE_COLOR = sf::Color(247, 244, 235, 175);
// Small helper text (e.g. the menu's "Use Left / Right arrows to move" hint).
inline constexpr unsigned int HINT_TEXT_SIZE = 20u;
// Volume slider styling.
inline const sf::Color SLIDER_TRACK_COLOR = sf::Color(210, 205, 195);
inline const sf::Color SLIDER_FILL_COLOR = sf::Color(224, 90, 60);
inline const sf::Color SLIDER_KNOB_COLOR = sf::Color(60, 60, 80);
inline constexpr float SLIDER_WIDTH = 260.0f;
inline constexpr float SLIDER_HEIGHT = 10.0f;
inline constexpr float SLIDER_KNOB_RADIUS = 12.0f;
} // namespace ui

} // namespace cfg
