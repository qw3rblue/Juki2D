#include <Juki2D/Application.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <string>

Juki2D::Application::Application(sf::Vector2u size, std::string title)
	: window(sf::VideoMode(size), title)
{
	window.setFramerateLimit(60);
}

const Juki2D::Input& Juki2D::Application::getInput() const {
	return input;
}

void Juki2D::Application::run() {
	sf::Clock clock;

	constexpr float fixedDeltaTime = 1.f / 60.f;
	float accumulator = 0.f;

	while (window.isOpen()) {
		float frameTime = clock.restart().asSeconds();

		frameTime = std::min(frameTime, 0.25f);
		accumulator += frameTime;

		while (auto event = window.pollEvent()) {
			if (event->is<sf::Event::Closed>()) {
				window.close();
			}

			if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
				input.press(keyEvent->code);
			}

			if (const auto* keyEvent = event->getIf<sf::Event::KeyReleased>()) {
				input.release(keyEvent->code);
			}

			if (event->is<sf::Event::FocusLost>()) {
				input.reset();
			}
		}

		if (!window.isOpen()) {
			break;
		}


		while (accumulator >= fixedDeltaTime) {
			onUpdate(fixedDeltaTime);
			input.finishUpdate();
			accumulator -= fixedDeltaTime;
		}

		window.clear();
		onRender(window);
		window.display();
	}
}