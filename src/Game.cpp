#include <Game.hpp>
#include <Collision.hpp>

Game::Game()
	: Juki2D::Application({1280, 720}, "Juki2D Sandbox"),
	camera(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f}))
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
	player.handleInput();
	player.update(dt);

	for (const auto& platform : platforms) {
		const sf::FloatRect playerBounds = player.getBounds();
		const sf::FloatRect platformBounds = platform.getGlobalBounds();

		if (Collision::isColliding(playerBounds, platformBounds)) {
			const Collision::Side side =
				Collision::getObjectCollisionSide(
					playerBounds, platformBounds
				);

			player.resolveCollision(platformBounds, side);
		}
	}

	std::vector<sf::FloatRect> obstacles;
	obstacles.reserve(platforms.size());

	for (const auto& platform : platforms) {
		obstacles.push_back(platform.getGlobalBounds());
	}

	const auto movement = Collision::moveSlide(
		player.getBounds(),
		player.getVelocity() * dt,
		obstacles
	);

	player.move(movement.displacement);

	for (const auto& normal : movement.normals) {
		player.respondToCollision(normal);
	}

	updateCamera();
}

void Game::onRender(sf::RenderWindow& window) {
	window.setView(camera);

	player.render(window);

	for (const auto& platform : platforms) {
		window.draw(platform);
	}
}