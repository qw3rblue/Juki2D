#include <Juki2D/Input.hpp>
#include <optional>
#include <cstddef>

namespace {
	std::optional<std::size_t> convertKeyToIndex(sf::Keyboard::Key key) {
		const int value = static_cast<int>(key);

		if (value < 0 || value >= sf::Keyboard::KeyCount) {
			return std::nullopt;
		}

		return value;
	}
}

bool Juki2D::Input::isHeld(sf::Keyboard::Key key) const {
	const auto index = convertKeyToIndex(key);
	return index && held.test(*index);
}

bool Juki2D::Input::wasPressed(sf::Keyboard::Key key) const {
	const auto index = convertKeyToIndex(key);
	return index && pressed.test(*index);
}

bool Juki2D::Input::wasReleased(sf::Keyboard::Key key) const {
	const auto index = convertKeyToIndex(key);
	return index && released.test(*index);
}

void Juki2D::Input::press(sf::Keyboard::Key key) {
	const auto index = convertKeyToIndex(key);

	if (!index) {
		return;
	}

	if (!held.test(*index)) {
		held.set(*index);
		pressed.set(*index);
	}
}

void Juki2D::Input::release(sf::Keyboard::Key key) {
	const auto index = convertKeyToIndex(key);

	if (!index) {
		return;
	}

	if (held.test(*index)) {
		held.reset(*index);
		released.set(*index);
	}
}

void Juki2D::Input::finishUpdate() {
	pressed.reset();
	released.reset();
}

void Juki2D::Input::reset() {
	pressed.reset();
	released.reset();
	held.reset();
}