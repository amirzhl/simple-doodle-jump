// ==================== Naming Guideline ====================
//  Classes / types      : PascalCase        (e.g. PlatformManager)
//  Functions / methods  : camelCase         (e.g. handleInput)
//  Local vars & params  : lower_snake_case  (e.g. world_top)
//  Member variables     : lower_snake_case_ (trailing underscore)
//  Constants            : UPPER_SNAKE_CASE  (e.g. MOVE_SPEED)
// ==========================================================

#include "ui/Slider.hpp"

#include "core/Constants.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <algorithm>
#include <cmath>

Slider::Slider(sf::Vector2f position, float width, float height, int min_value,
               int max_value)
	: position_(position), width_(width), height_(height),
	  min_value_(min_value), max_value_(max_value), value_(min_value) {
	track_.setSize(sf::Vector2f(width_, height_));
	track_.setPosition(position_);
	track_.setFillColor(cfg::ui::SLIDER_TRACK_COLOR);

	fill_.setPosition(position_);
	fill_.setFillColor(cfg::ui::SLIDER_FILL_COLOR);

	const float diameter = cfg::ui::SLIDER_KNOB_RADIUS * 2.0f;
	knob_.setSize(sf::Vector2f(diameter, diameter));
	knob_.setOrigin(cfg::ui::SLIDER_KNOB_RADIUS, cfg::ui::SLIDER_KNOB_RADIUS);
	knob_.setFillColor(cfg::ui::SLIDER_KNOB_COLOR);

	refresh();
}

void Slider::setValue(int value) {
	value_ = std::max(min_value_, std::min(max_value_, value));
	refresh();
}

void Slider::draw(sf::RenderTarget& target) const {
	target.draw(track_);
	target.draw(fill_);
	target.draw(knob_);
}

bool Slider::handleMousePress(sf::Vector2f point) {
	// Generous hit area: the whole track plus a knob-radius margin all around.
	const float margin = cfg::ui::SLIDER_KNOB_RADIUS;
	const bool inside = point.x >= position_.x - margin &&
	                    point.x <= position_.x + width_ + margin &&
	                    point.y >= position_.y - margin &&
	                    point.y <= position_.y + height_ + margin;
	if (!inside) {
		return false;
	}
	dragging_ = true;
	updateFromX(point.x);
	return true;
}

void Slider::handleMouseMove(sf::Vector2f point) {
	if (dragging_) {
		updateFromX(point.x);
	}
}

void Slider::updateFromX(float x) {
	float fraction = (x - position_.x) / width_;
	fraction = std::max(0.0f, std::min(1.0f, fraction));
	const float range = static_cast<float>(max_value_ - min_value_);
	value_ = min_value_ + static_cast<int>(std::lround(fraction * range));
	refresh();
}

void Slider::refresh() {
	const float range = static_cast<float>(max_value_ - min_value_);
	const float fraction =
		range > 0.0f ? static_cast<float>(value_ - min_value_) / range : 0.0f;
	const float fill_width = width_ * fraction;
	fill_.setSize(sf::Vector2f(fill_width, height_));
	knob_.setPosition(position_.x + fill_width, position_.y + height_ / 2.0f);
}
