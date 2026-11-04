#pragma once
#include <SFML/Window/Event.hpp>
#include <bitset>
#include <vector>

namespace Juki2D {
	enum class PointerEventType {
		Pressed,
		Released,
		Cancelled
	};

	struct PointerEvent {
		PointerEventType type;
		sf::Mouse::Button button;
		sf::Vector2i position;
	};

	class Input {
	private:
		std::bitset<sf::Keyboard::KeyCount> held;
		std::bitset<sf::Keyboard::KeyCount> pressed;
		std::bitset<sf::Keyboard::KeyCount> released;
		std::bitset<sf::Mouse::ButtonCount> mouseHeld;
		std::bitset<sf::Mouse::ButtonCount> mousePressed;
		std::bitset<sf::Mouse::ButtonCount> mouseReleased;
		std::vector<PointerEvent> pointerEvents;
		sf::Vector2i mousePosition = { 0, 0 };
		sf::Vector2u windowSize = { 0, 0 };
		bool mouseInside = false;

	public:
		bool isHeld(sf::Keyboard::Key key) const;
		bool wasPressed(sf::Keyboard::Key key) const;
		bool wasReleased(sf::Keyboard::Key key) const;
		void press(sf::Keyboard::Key key);
		void release(sf::Keyboard::Key key);

		bool isHeld(sf::Mouse::Button button) const;
		bool wasPressed(sf::Mouse::Button button) const;
		bool wasReleased(sf::Mouse::Button button) const;
		sf::Vector2i getMousePosition() const;
		sf::Vector2u getWindowSize() const;
		bool isMouseInside() const;
		const std::vector<PointerEvent>& getPointerEvents() const;

		void handleEvent(const sf::Event& event);
		void setWindowSize(sf::Vector2u value);
		void finishUpdate();
		void reset();
	};
}
