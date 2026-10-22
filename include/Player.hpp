#pragma once
#include <SFML/Graphics.hpp>
#include <Collision.hpp>
#include <Juki2D/Body2D.hpp>
#include <vector>

class Player {
private:
	sf::RectangleShape visual;
	Juki2D::Body2D body;

	float moveSpeed = 250.f;
	float jumpForce = 600.f;

public:
	Player();

	void move(sf::Vector2f displacement);
	void respondToCollision(sf::Vector2f normal);

	sf::FloatRect getBounds() const;
	sf::Vector2f getVelocity() const;

	void resolveCollision(sf::FloatRect object, Collision::Side side);
	void handleInput();
	void update(float dt, const std::vector<sf::FloatRect>& obstacles);
	void render(sf::RenderWindow& window);
};