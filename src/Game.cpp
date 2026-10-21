#include "Game.hpp"
#include "Collision.hpp"

Game::Game()
	: window(sf::VideoMode({ 1280, 720 }), "Platformer Game :D"),
	camera(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f}))
{
	window.setFramerateLimit(60);

	sf::RectangleShape floor;
	sf::RectangleShape platform1;
	sf::RectangleShape platform2;
	sf::RectangleShape platform3;

	floor.setSize({ 2000.f, 30.f });
	floor.setPosition({ 0.f, 640.f });

	platforms.push_back(floor);
	platforms.push_back(platform1);
	platforms.push_back(platform2);
	platforms.push_back(platform3);

	for (int i = 1; i <= 3; i++) {
		platforms[i].setSize({ 220.f, 18.f });
		platforms[i].setPosition({ 150.f + (i - 1.f) * 310.f, 520.f - (i - 1.f) * 100.f });
	}
}

void Game::updateCamera() {
	sf::FloatRect bounds = player.getBounds();

	sf::Vector2f playerCenter = {
		bounds.position.x + bounds.size.x / 2.f,
		bounds.position.y + bounds.size.y / 2.f
	};

	camera.setCenter(playerCenter);
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
	player.update(dt);

	sf::FloatRect playerBounds = player.getBounds();

	// generalized collision checking

	for (int i = 0; i <= platforms.size() - 1; i++) {
		sf::FloatRect currentBounds = platforms[i].getGlobalBounds();

		if (Collision::isColliding(playerBounds, currentBounds)) {
			char side = Collision::getObjectCollisionSide(playerBounds, currentBounds)[0];
			player.resolveCollision(currentBounds, side);
		}
	}

	updateCamera();
}

void Game::render() {
	window.clear();
	window.setView(camera);

	player.render(window);

	for (int i = 0; i <= platforms.size() - 1; i++) {
		window.draw(platforms[i]);
	}

	window.display();
}

void Game::run() {
	sf::Clock clock;

	constexpr float fixedDeltaTime = 1.f / 60.f;
	float accumulator = 0.f;

	while (window.isOpen()) {
		float frameTime = clock.restart().asSeconds();

		// lag prevention they said
		frameTime = std::min(frameTime, 0.25f);

		accumulator += frameTime;

		processEvents();

		while (accumulator >= fixedDeltaTime) {
			update(fixedDeltaTime);
			accumulator -= fixedDeltaTime;
		}

		render();
	}
}