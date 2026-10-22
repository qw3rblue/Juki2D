#pragma once
#include <SFML/Graphics.hpp>
#include <Collision.hpp>
#include <Juki2D/Body2D.hpp>
#include <Juki2D/Input.hpp>
#include <vector>

class Player {
private:
	sf::RectangleShape visual;
	Juki2D::Body2D body;

	float moveSpeed = 250.f;
	float jumpForce = 600.f;

public:
	Player();

	sf::FloatRect getBounds() const;

	void handleInput(const Juki2D::Input&);
	void update(float dt, const std::vector<sf::FloatRect>& obstacles);
	void render(sf::RenderWindow& window);
};