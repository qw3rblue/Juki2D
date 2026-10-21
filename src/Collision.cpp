#include "Collision.hpp"

bool Collision::isColliding(
	const sf::FloatRect& a,
	const sf::FloatRect& b
) {
	return a.findIntersection(b).has_value();
}

std::string Collision::getObjectCollisionSide(
	const sf::FloatRect& a,
	const sf::FloatRect& b
) {
	float aCenterX = a.position.x + (a.size.x / 2.0f);
	float aCenterY = a.position.y + (a.size.y / 2.0f);
	float bCenterX = b.position.x + (b.size.x / 2.0f);
	float bCenterY = b.position.y + (b.size.y / 2.0f);

	float distX = aCenterX - bCenterX;
	float distY = aCenterY - bCenterY;

	float minDistX = (a.size.x / 2.0f) + (b.size.x / 2.0f);
	float minDistY = (a.size.y / 2.0f) + (b.size.y / 2.0f);

	float xOverlap = minDistX - std::abs(distX);
	float yOverlap = minDistY - std::abs(distY);

	if (xOverlap < yOverlap) {
		if (distX > 0) {
			return "right";
		}
		else {
			return "left";
		}
	}
	else {
		if (distY > 0) {
			return "bottom";
		}
		else {
			return "top";
		}
	}
}