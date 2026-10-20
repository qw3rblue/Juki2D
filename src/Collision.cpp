#include "Collision.hpp"

bool Collision::isColliding(
	const sf::FloatRect& a,
	const sf::FloatRect & b
) {
	return a.findIntersection(b).has_value();
}