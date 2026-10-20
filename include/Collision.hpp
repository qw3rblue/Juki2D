#pragma once
#include <SFML/Graphics.hpp>

class Collision {
public:
	static bool isColliding(
		const sf::FloatRect& a,
		const sf::FloatRect& b
	);
};