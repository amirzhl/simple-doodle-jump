// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

namespace sf {
class RenderTarget;
}

// ---------------------------------------------------------------------------
//  Slider  -- a small, reusable horizontal slider used by the Settings screen
//  to pick the master volume. It renders a track, a coloured fill and a
//  draggable knob, and maps a mouse x-position to an integer value in
//  [min, max]. Hit-testing uses the stored geometry (not sprite bounds) so it
//  behaves correctly regardless of texture sizes.
// ---------------------------------------------------------------------------
class Slider {
public:
	Slider(sf::Vector2f position, float width, float height, int min_value,
	       int max_value);

	void setValue(int value);
	int value() const { return value_; }

	void draw(sf::RenderTarget& target) const;

	// Begin a drag if `point` is on the slider; returns true when handled and
	// immediately snaps the value to the click position.
	bool handleMousePress(sf::Vector2f point);
	// While dragging, follow the mouse x-position.
	void handleMouseMove(sf::Vector2f point);
	void endDrag() { dragging_ = false; }
	bool isDragging() const { return dragging_; }

private:
	void updateFromX(float x);
	void refresh(); // reposition fill + knob from the current value

	sf::Vector2f position_;
	float width_;
	float height_;
	int min_value_;
	int max_value_;
	int value_;
	bool dragging_ = false;

	sf::RectangleShape track_;
	sf::RectangleShape fill_;
	sf::RectangleShape knob_;
};
