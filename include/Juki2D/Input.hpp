#pragma once
#include <bitset>
#include <SFML/Window/Keyboard.hpp>

namespace Juki2D {
	class Input {
	private:
		std::bitset<sf::Keyboard::KeyCount> held;
		std::bitset<sf::Keyboard::KeyCount> pressed;
		std::bitset<sf::Keyboard::KeyCount> released;

	public:
		bool isHeld(sf::Keyboard::Key key) const;
		bool wasPressed(sf::Keyboard::Key key) const;
		bool wasReleased(sf::Keyboard::Key key) const;
		void press(sf::Keyboard::Key key);
		void release(sf::Keyboard::Key key);
		void finishUpdate();
		void reset();
	};
}