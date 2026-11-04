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
	mousePressed.reset();
	mouseReleased.reset();
	pointerEvents.clear();
}

void Juki2D::Input::reset() {
	finishUpdate();
	held.reset();
	mouseHeld.reset();
	mouseInside = false;
	pointerEvents.push_back({ PointerEventType::Cancelled, sf::Mouse::Button::Left, mousePosition });
}

bool Juki2D::Input::isHeld(sf::Mouse::Button button) const {
	const auto value = static_cast<std::size_t>(button);
	return value < sf::Mouse::ButtonCount && mouseHeld.test(value);
}

bool Juki2D::Input::wasPressed(sf::Mouse::Button button) const {
	const auto value = static_cast<std::size_t>(button);
	return value < sf::Mouse::ButtonCount && mousePressed.test(value);
}

bool Juki2D::Input::wasReleased(sf::Mouse::Button button) const {
	const auto value = static_cast<std::size_t>(button);
	return value < sf::Mouse::ButtonCount && mouseReleased.test(value);
}

sf::Vector2i Juki2D::Input::getMousePosition() const {
	return mousePosition;
}

sf::Vector2u Juki2D::Input::getWindowSize() const {
	return windowSize;
}

bool Juki2D::Input::isMouseInside() const {
	return mouseInside;
}

const std::vector<Juki2D::PointerEvent>& Juki2D::Input::getPointerEvents() const {
	return pointerEvents;
}

void Juki2D::Input::setWindowSize(sf::Vector2u value) {
	windowSize = value;
}

void Juki2D::Input::handleEvent(const sf::Event& event) {
	if (const auto* key = event.getIf<sf::Event::KeyPressed>()) {
		press(key->code);
	}
	else if (const auto* key = event.getIf<sf::Event::KeyReleased>()) {
		release(key->code);
	}
	else if (const auto* mouse = event.getIf<sf::Event::MouseMoved>()) {
		mousePosition = mouse->position;
		mouseInside = true;
	}
	else if (const auto* mouse = event.getIf<sf::Event::MouseButtonPressed>()) {
		mousePosition = mouse->position;
		mouseInside = true;
		const auto index = static_cast<std::size_t>(mouse->button);

		if (index < sf::Mouse::ButtonCount && !mouseHeld.test(index)) {
			mouseHeld.set(index);
			mousePressed.set(index);
			pointerEvents.push_back({ PointerEventType::Pressed, mouse->button, mousePosition });
		}
	}
	else if (const auto* mouse = event.getIf<sf::Event::MouseButtonReleased>()) {
		mousePosition = mouse->position;
		const auto index = static_cast<std::size_t>(mouse->button);

		if (index < sf::Mouse::ButtonCount && mouseHeld.test(index)) {
			mouseHeld.reset(index);
			mouseReleased.set(index);
			pointerEvents.push_back({ PointerEventType::Released, mouse->button, mousePosition });
		}
	}
	else if (event.is<sf::Event::MouseEntered>()) {
		mouseInside = true;
	}
	else if (event.is<sf::Event::MouseLeft>()) {
		mouseInside = false;
	}
	else if (event.is<sf::Event::FocusLost>()) {
		reset();
	}
}
