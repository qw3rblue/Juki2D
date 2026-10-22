#include <Juki2D/Body2D.hpp>
#include <Juki2D/Collision.hpp>

Juki2D::Body2D::Body2D(sf::Vector2f position,sf::Vector2f size)
	: bounds(position, size) 
{
	
}

sf::FloatRect Juki2D::Body2D::getBounds() const {
	return bounds;
}

sf::Vector2f Juki2D::Body2D::getVelocity() const {
	return velocity;
}

void Juki2D::Body2D::setVelocity(sf::Vector2f newVelocity) {
	velocity = newVelocity;
}

void Juki2D::Body2D::setGravity(float newGravity) {
	gravity = newGravity;
}

bool Juki2D::Body2D::isGrounded() const {
	return grounded;
}

void Juki2D::Body2D::update(float dt, const std::vector<sf::FloatRect>& obstacles) {
	grounded = false;
	velocity.y += gravity * dt;

	for (const auto& obstacle : obstacles) {
		if (!Collision::isColliding(bounds, obstacle)) {
			continue;
		}

		const auto side = Collision::getObjectCollisionSide(bounds, obstacle);

		switch (side) {
		case Collision::Side::Top:
			bounds.position.y = obstacle.position.y - bounds.size.y;
			velocity.y = 0.f;
			grounded = true;
			break;

		case Collision::Side::Bottom:
			bounds.position.y = obstacle.position.y + obstacle.size.y;
			velocity.y = 0.f;
			break;

		case Collision::Side::Left:
			bounds.position.x = obstacle.position.x - bounds.size.x;
			velocity.x = 0.f;
			break;

		case Collision::Side::Right:
			bounds.position.x = obstacle.position.x + obstacle.size.x;
			velocity.x = 0.f;
			break;
		}
	}

	const auto movement = Collision::moveSlide(bounds, velocity * dt, obstacles);

	bounds.position += movement.displacement;

	for (const auto& normal : movement.normals) {
		if (velocity.x * normal.x < 0.f) {
			velocity.x = 0.f;
		}

		if (velocity.y * normal.y < 0.f) {
			velocity.y = 0.f;
		}

		if (normal.y < 0.f) {
			grounded = true;
		}
	}
}