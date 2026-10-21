#pragma once
#include <SFML/Graphics.hpp>
#include <Collision.hpp>

class Player {
private:
	sf::RectangleShape body;
	sf::Vector2f velocity;

	float moveSpeed = 250.f;
	float jumpForce = 600.f;
	float gravity = 1200.f;

	bool grounded = false;

public:
	Player();

	sf::FloatRect getBounds() const;
	sf::Vector2f getVelocity() const;

	void resolveCollision(sf::FloatRect object, char side);
	void handleInput();
	void update(float dt);
	void render(sf::RenderWindow& window);
};