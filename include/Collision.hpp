#pragma once
#include <SFML/Graphics.hpp>

class Collision {
public:
	enum class Side {
		Top, Bottom, Left, Right
	};

	static bool isColliding(
		const sf::FloatRect& a,
		const sf::FloatRect& b
	);

	static Side getObjectCollisionSide(
		const sf::FloatRect& a,
		const sf::FloatRect& b
	);
};