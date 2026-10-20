#include "Game.hpp"
#include "Collision.hpp"

Game::Game()
	: window(sf::VideoMode({ 1280, 720 }), "Platformer Game :D"),
	platform(sf::Vector2f({300.f, 30.f}))
{
	window.setFramerateLimit(60);

	platform.setPosition({ 400.f, 550.f });
	platform.setFillColor(sf::Color::Blue);
}

void Game::processEvents() {
	while (auto event = window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			window.close();
		}
	}
}

void Game::update(float dt) {
	player.handleInput();

	sf::FloatRect previousBounds = player.getBounds();

	player.update(dt);

	sf::FloatRect playerBounds = player.getBounds();
	sf::FloatRect platformBounds = platform.getGlobalBounds();

	float previousBottom = previousBounds.position.y + previousBounds.size.y;
	float platformTop = platformBounds.position.y;

	if (Collision::isColliding(
		playerBounds,
		platformBounds
	) && previousBottom <= platformTop && player.getVelocity().y >= 0.f) {
		player.landOn(platformTop);
	}
}

void Game::render() {
	window.clear();

	window.draw(platform);
	player.render(window);

	window.display();
}

void Game::run() {
	sf::Clock clock;

	while (window.isOpen()) {
		float dt = clock.restart().asSeconds();

		processEvents();
		update(dt);
		render();
	}
}