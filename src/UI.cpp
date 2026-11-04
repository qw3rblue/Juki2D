#include <Juki2D/UI.hpp>
#include <algorithm>
#include <cmath>
#include <array>
#include <numbers>
#include <stdexcept>

namespace {
	void validateSize(sf::Vector2f size) {
		if (!std::isfinite(size.x) || !std::isfinite(size.y) || size.x < 0.f || size.y < 0.f) {
			throw std::invalid_argument("UI size must be finite and nonnegative.");
		}
	}

	void validateSpacing(float value) {
		if (!std::isfinite(value) || value < 0.f) {
			throw std::invalid_argument("UI spacing must be finite and nonnegative.");
		}
	}
}

sf::ConvexShape Juki2D::makeRoundedRectangle(sf::Vector2f size, float radius) {
	validateSize(size);
	validateSpacing(radius);
	radius = std::min(radius, std::min(size.x, size.y) / 2.f);
	constexpr std::size_t segments = 10;
	sf::ConvexShape shape(4 * (segments + 1));
	const std::array<sf::Vector2f, 4> centers = {
		sf::Vector2f{ size.x - radius, radius },
		sf::Vector2f{ size.x - radius, size.y - radius },
		sf::Vector2f{ radius, size.y - radius },
		sf::Vector2f{ radius, radius }
	};

	for (std::size_t corner = 0; corner < centers.size(); ++corner) {
		for (std::size_t step = 0; step <= segments; ++step) {
			const float angle = (static_cast<float>(corner) - 1.f
				+ static_cast<float>(step) / segments) * std::numbers::pi_v<float> / 2.f;
			shape.setPoint(corner * (segments + 1) + step, {
				centers[corner].x + std::cos(angle) * radius,
				centers[corner].y + std::sin(angle) * radius
			});
		}
	}

	return shape;
}

sf::FloatRect Juki2D::UILayout::anchor(const sf::FloatRect& parent, sf::Vector2f size,
	Anchor anchor, sf::Vector2f offset, float padding) {
	validateSize(parent.size);
	validateSize(size);
	validateSpacing(padding);
	const int index = static_cast<int>(anchor);
	const sf::Vector2f factor = { (index % 3) / 2.f, (index / 3) / 2.f };
	const sf::Vector2f available = {
		std::max(0.f, parent.size.x - padding * 2.f),
		std::max(0.f, parent.size.y - padding * 2.f)
	};

	return {
		{
			parent.position.x + padding + (available.x - size.x) * factor.x + offset.x,
			parent.position.y + padding + (available.y - size.y) * factor.y + offset.y
		},
		size
	};
}

std::vector<sf::FloatRect> Juki2D::UILayout::stack(const sf::FloatRect& parent,
	std::span<const sf::Vector2f> sizes, LayoutDirection direction,
	float spacing, float padding, TextAlignment alignment) {
	validateSize(parent.size);
	validateSpacing(spacing);
	validateSpacing(padding);
	std::vector<sf::FloatRect> result;
	float cursor = padding;
	float factor = 0.f;

	if (alignment == TextAlignment::Center) {
		factor = 0.5f;
	}
	else if (alignment == TextAlignment::Right) {
		factor = 1.f;
	}

	for (const auto size : sizes) {
		validateSize(size);
		sf::Vector2f position = parent.position;

		if (direction == LayoutDirection::Vertical) {
			position.x += padding + (std::max(0.f, parent.size.x - padding * 2.f) - size.x) * factor;
			position.y += cursor;
			cursor += size.y + spacing;
		}
		else {
			position.x += cursor;
			position.y += padding + (std::max(0.f, parent.size.y - padding * 2.f) - size.y) * factor;
			cursor += size.x + spacing;
		}

		result.push_back({ position, size });
	}

	return result;
}

Juki2D::UICanvas::UICanvas(sf::Vector2f size)
	: size(size), view(sf::FloatRect({ 0.f, 0.f }, size))
{
	validateSize(size);

	if (size.x == 0.f || size.y == 0.f) {
		throw std::invalid_argument("UI canvas size must be greater than zero.");
	}
}

void Juki2D::UICanvas::setWindowSize(sf::Vector2u value) {
	windowSize = value;
	viewport = { { 0, 0 }, { 0, 0 } };

	if (value.x == 0 || value.y == 0) {
		return;
	}

	const float scale = std::min(value.x / size.x, value.y / size.y);
	const sf::Vector2i extent = {
		std::max(1, static_cast<int>(std::round(size.x * scale))),
		std::max(1, static_cast<int>(std::round(size.y * scale)))
	};
	viewport = {
		{ (static_cast<int>(value.x) - extent.x) / 2, (static_cast<int>(value.y) - extent.y) / 2 },
		extent
	};
	view.setViewport({
		{ viewport.position.x / static_cast<float>(value.x), viewport.position.y / static_cast<float>(value.y) },
		{ extent.x / static_cast<float>(value.x), extent.y / static_cast<float>(value.y) }
	});
}

void Juki2D::UICanvas::apply(sf::RenderWindow& window) {
	setWindowSize(window.getSize());
	window.setView(view);
}

std::optional<sf::Vector2f> Juki2D::UICanvas::mapPixel(sf::Vector2i pixel) const {
	if (windowSize.x == 0 || windowSize.y == 0 || !viewport.contains(pixel)) {
		return std::nullopt;
	}

	return sf::Vector2f{
		(pixel.x - viewport.position.x) * size.x / viewport.size.x,
		(pixel.y - viewport.position.y) * size.y / viewport.size.y
	};
}

sf::FloatRect Juki2D::UICanvas::getBounds() const {
	return { { 0.f, 0.f }, size };
}

Juki2D::Button::Button(FontStore& fonts, const std::string& fontPath, const sf::String& string)
	: label(fonts, fontPath, string, 20)
{
	setBounds({ { 0.f, 0.f }, { 180.f, 44.f } });
}

void Juki2D::Button::setBounds(const sf::FloatRect& bounds) {
	validateSize(bounds.size);
	this->bounds = bounds;
	background = makeRoundedRectangle(bounds.size, style.cornerRadius);
	background.setPosition(bounds.position);
	updateVisual();
}

void Juki2D::Button::setString(const sf::String& value) {
	label.setString(value);
	updateVisual();
}

void Juki2D::Button::setSize(unsigned value) {
	label.setSize(value);
	updateVisual();
}

void Juki2D::Button::setStyle(const ButtonStyle& value) {
	validateSpacing(value.cornerRadius);
	validateSpacing(value.outlineThickness);
	style = value;
	setBounds(bounds);
}

void Juki2D::Button::setEnabled(bool value) {
	enabled = value;

	if (!enabled) {
		hovered = false;
		armed = false;
		clicked = false;
	}

	updateVisual();
}

void Juki2D::Button::update(const Input& input, const UICanvas& canvas) {
	clicked = false;

	if (!enabled) {
		return;
	}

	for (const auto& event : input.getPointerEvents()) {
		if (event.type == PointerEventType::Cancelled) {
			armed = false;
			clicked = false;
			continue;
		}

		if (event.button != sf::Mouse::Button::Left) {
			continue;
		}

		const auto point = canvas.mapPixel(event.position);
		const bool inside = point && getBounds().contains(*point);

		if (event.type == PointerEventType::Pressed) {
			armed = inside;
		}
		else if (event.type == PointerEventType::Released) {
			clicked = clicked || (armed && inside);
			armed = false;
		}
	}

	const auto point = canvas.mapPixel(input.getMousePosition());
	hovered = input.isMouseInside() && point && getBounds().contains(*point);

	if (!input.isHeld(sf::Mouse::Button::Left)) {
		armed = false;
	}

	updateVisual();
}

void Juki2D::Button::updateVisual() {
	sf::Color color = style.normal;

	if (!enabled) {
		color = style.disabled;
	}
	else if (armed && hovered) {
		color = style.pressed;
	}
	else if (hovered) {
		color = style.hovered;
	}

	background.setFillColor(color);
	background.setOutlineColor(style.outline);
	background.setOutlineThickness(style.outlineThickness);
	label.setColor(style.text);
	label.setPosition({ 0.f, 0.f });
	const auto textBounds = label.getBounds();
	const auto bounds = getBounds();
	label.setPosition({
		bounds.position.x + (bounds.size.x - textBounds.size.x) / 2.f - textBounds.position.x,
		bounds.position.y + (bounds.size.y - textBounds.size.y) / 2.f - textBounds.position.y
	});
}

sf::FloatRect Juki2D::Button::getBounds() const {
	return bounds;
}

bool Juki2D::Button::isHovered() const {
	return hovered;
}

bool Juki2D::Button::isPressed() const {
	return armed && hovered;
}

bool Juki2D::Button::wasClicked() const {
	return clicked;
}

void Juki2D::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(background, states);
	target.draw(label, states);
}
