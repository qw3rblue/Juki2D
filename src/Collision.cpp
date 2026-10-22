#include <Collision.hpp>
#include <cmath>
#include <limits>
#include <algorithm>

bool Collision::isColliding(
	const sf::FloatRect& a,
	const sf::FloatRect& b
) {
	return a.findIntersection(b).has_value();
}

std::optional<Collision::SweepHit> Collision::sweepAABB(
	const sf::FloatRect& moving,
	sf::Vector2f displacement,
	const sf::FloatRect& obstacle
) {
	float movingLeft = moving.position.x;
	float movingRight = moving.position.x + moving.size.x;
	float obsLeft = obstacle.position.x;
	float obsRight = obstacle.position.x + obstacle.size.x;

	float movingUp = moving.position.y;
	float movingDown = moving.position.y + moving.size.y;
	float obsUp = obstacle.position.y;
	float obsDown = obstacle.position.y + obstacle.size.y;

	float xEntry; float xExit;
	float yEntry; float yExit;

	const float infinity = std::numeric_limits<float>::infinity();

	if (displacement.x > 0.f) { // right
		xEntry = (obsLeft - movingRight) / displacement.x;
		xExit = (obsRight - movingLeft) / displacement.x;
	}
	else if (displacement.x < 0.f) { // left
		xEntry = (obsRight - movingLeft) / displacement.x;
		xExit = (obsLeft - movingRight) / displacement.x;
	}
	else {
		if (movingRight <= obsLeft || movingLeft >= obsRight) {
			return std::nullopt;
		}

		xEntry = -infinity;
		xExit = infinity;
	}

	if (displacement.y > 0.f) { // down
		yEntry = (obsUp - movingDown) / displacement.y;
		yExit = (obsDown - movingUp) / displacement.y;
	}
	else if (displacement.y < 0.f) { // up
		yEntry = (obsDown - movingUp) / displacement.y;
		yExit = (obsUp - movingDown) / displacement.y;
	}
	else {
		if (movingDown <= obsUp || movingUp >= obsDown) {
			return std::nullopt;
		}

		yEntry = -infinity;
		yExit = infinity;
	}

	float entryTime = std::max(xEntry, yEntry);
	float exitTime = std::min(xExit, yExit);

	if (entryTime > exitTime
		|| entryTime < 0.f
		|| entryTime > 1.f) {
		return std::nullopt;
	}
	else {
		sf::Vector2f normal;

		if (xEntry > yEntry) {
			if (displacement.x > 0.f) { // right, surface left
				normal = { -1.f, 0.f };
			}
			else { // left, surface right
				normal = { 1.f, 0.f };
			}
		}
		else {
			if (displacement.y > 0.f) { // downward, surface up
				normal = { 0.f, -1.f };
			}
			else { // upward, surface down
				normal = { 0.f, 1.f };
			}
		}

		return SweepHit{ entryTime, normal }; 
	}
}

Collision::Side Collision::getObjectCollisionSide(
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
			return Side::Right;
		}
		else {
			return Side::Left;
		}
	}
	else {
		if (distY > 0) {
			return Side::Bottom;
		}
		else {
			return Side::Top;
		}
	}
}