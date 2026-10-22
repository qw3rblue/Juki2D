#include <Juki2D/Input.hpp>

bool Juki2D::Input::isHeld(sf::Keyboard::Key key) const {
	return held.contains(key);
}

bool Juki2D::Input::wasPressed(sf::Keyboard::Key key) const {
	return pressed.contains(key);
}

bool Juki2D::Input::wasReleased(sf::Keyboard::Key key) const {
	return released.contains(key);
}

void Juki2D::Input::press(sf::Keyboard::Key key) {
	if (!(held.contains(key))) {
		held.insert(key);
		pressed.insert(key);
	}
}

void Juki2D::Input::release(sf::Keyboard::Key key) {
	if (held.contains(key)) {
		held.erase(key);
		released.insert(key);
	}
}

void Juki2D::Input::finishUpdate() {
	pressed.clear();
	released.clear();
}

void Juki2D::Input::reset() {
	pressed.clear();
	released.clear();
	held.clear();
}