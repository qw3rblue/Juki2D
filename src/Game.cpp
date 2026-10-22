#include <Game.hpp>
#include <Collision.hpp>

Game::Game()
	: Juki2D::Application({1280, 720}, "Juki2D Sandbox"),
	camera(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f})),
	player(textures)
{
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

	obstacleBounds.reserve(platforms.size());

	for (const auto& platform : platforms) {
		obstacleBounds.push_back(platform.getGlobalBounds());
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

void Game::onUpdate(float dt) {
	player.handleInput(getInput());
	player.update(dt, obstacleBounds);
	updateCamera();
}

void Game::onRender(sf::RenderWindow& window) {
	window.setView(camera);

	player.render(window);

	for (const auto& platform : platforms) {
		window.draw(platform);
	}
}