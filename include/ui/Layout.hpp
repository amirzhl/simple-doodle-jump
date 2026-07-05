// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf {
class Text;
class Sprite;
class Texture;
} // namespace sf

// ---------------------------------------------------------------------------
//  ui::Layout  -- tiny, shared layout helpers used by every screen. Extracted
//  into one place so the identical text-centring and background-stretching
//  code is written exactly once (DRY) instead of being copied into each State.
// ---------------------------------------------------------------------------
namespace ui {

// Horizontally centre `text` around `center_x`, with its top edge at `y`.
void centerTextHorizontally(sf::Text& text, float center_x, float y);

// Scale `sprite` (using `texture`) so it exactly fills the game window.
void stretchToWindow(sf::Sprite& sprite, const sf::Texture& texture);

} // namespace ui
