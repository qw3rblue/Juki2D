#pragma once
#include <Juki2D/UI.hpp>
#include <algorithm>
#include <cmath>

namespace Juki2D {
	class Slider : public sf::Drawable {
	private:
		sf::FloatRect bounds{ { 0.f, 0.f }, { 280.f, 28.f } };
		sf::RectangleShape track;
		sf::RectangleShape fill;
		sf::CircleShape thumb{ 8.f };
		int value = 100;
		bool dragging = false;
		bool changed = false;
		bool finished = false;

		void updateVisual() {
			const float y = bounds.position.y + bounds.size.y / 2.f;
			const float width = bounds.size.x * value / 100.f;
			track.setPosition({ bounds.position.x, y - 3.f });
			track.setSize({ bounds.size.x, 6.f });
			track.setFillColor(sf::Color(100, 107, 118, 95));
			fill.setPosition(track.getPosition());
			fill.setSize({ width, 6.f });
			fill.setFillColor(sf::Color(87, 110, 139, 220));
			thumb.setOrigin({ 8.f, 8.f });
			thumb.setPosition({ bounds.position.x + width, y });
			thumb.setFillColor(dragging ? sf::Color(201, 219, 235) : sf::Color(250, 249, 246));
			thumb.setOutlineColor(sf::Color(87, 100, 120, 210));
			thumb.setOutlineThickness(1.f);
		}

		void setFromPoint(sf::Vector2f point) {
			const int next = static_cast<int>(std::round(
				std::clamp((point.x - bounds.position.x) / bounds.size.x, 0.f, 1.f) * 100.f));
			changed = changed || next != value;
			value = next;
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
			target.draw(track, states);
			target.draw(fill, states);
			target.draw(thumb, states);
		}

	public:
		void setBounds(const sf::FloatRect& area) {
			if (area.size.x <= 0.f || area.size.y <= 0.f) {
				return;
			}

			bounds = area;
			updateVisual();
		}

		void setValue(int next) {
			value = std::clamp(next, 0, 100);
			updateVisual();
		}

		int getValue() const {
			return value;
		}

		bool hasChanged() const {
			return changed;
		}

		bool didFinish() const {
			return finished;
		}

		bool isDragging() const {
			return dragging;
		}

		void cancelDrag() {
			dragging = false;
			changed = false;
			finished = false;
			updateVisual();
		}

		void update(const Input& input, const UICanvas& canvas) {
			changed = false;
			finished = false;

			for (const auto& event : input.getPointerEvents()) {
				if (event.type == PointerEventType::Cancelled) {
					finished = finished || dragging;
					dragging = false;
					continue;
				}

				if (event.button != sf::Mouse::Button::Left) {
					continue;
				}

				const auto point = canvas.mapPixel(event.position);

				if (event.type == PointerEventType::Pressed && point && bounds.contains(*point)) {
					dragging = true;
					setFromPoint(*point);
				}
				else if (event.type == PointerEventType::Released && dragging) {
					if (point) {
						setFromPoint(*point);
					}

					dragging = false;
					finished = true;
				}
			}

			if (dragging) {
				const auto point = canvas.mapPixel(input.getMousePosition());

				if (point) {
					setFromPoint(*point);
				}

				if (!input.isHeld(sf::Mouse::Button::Left)) {
					dragging = false;
					finished = true;
				}
			}

			updateVisual();
		}
	};
}
