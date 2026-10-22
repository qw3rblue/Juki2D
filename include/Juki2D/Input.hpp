#pragma once
#include <set>
#include <SFML/Window/Keyboard.hpp>

namespace Juki2D {
	class Input {
	private:
		std::set<sf::Keyboard::Key> held;
		std::set<sf::Keyboard::Key> pressed;
		std::set<sf::Keyboard::Key> released;

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