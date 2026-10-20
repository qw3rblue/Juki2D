#pragma once
#include <SFML/Graphics.hpp>

class Player {
private:
	sf::RectangleShape body;
	sf::Vector2f velocity;

	float moveSpeed = 250.f;
	float jumpForce = 500.f;
	float gravity = 2000.f;

	bool grounded = false;

public:
	Player();

	sf::FloatRect getBounds() const;
	sf::Vector2f getVelocity() const;
	void landOn(float platformTop);
	void handleInput();
	void update(float dt);
	void render(sf::RenderWindow& window);
};