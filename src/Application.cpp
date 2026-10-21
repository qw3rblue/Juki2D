#include <Juki2D/Application.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <string>

Juki2D::Application::Application(sf::Vector2u size, std::string title)
	: window(sf::VideoMode(size), title)
{
	window.setFramerateLimit(60);
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
		}

		if (!window.isOpen()) {
			break;
		}


		while (accumulator >= fixedDeltaTime) {
			onUpdate(fixedDeltaTime);
			accumulator -= fixedDeltaTime;
		}

		window.clear();
		onRender(window);
		window.display();
	}
}