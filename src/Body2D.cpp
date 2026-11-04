#include <Juki2D/Body2D.hpp>
#include <Juki2D/Collision.hpp>
#include <algorithm>
#include <unordered_set>
#include <stdexcept>
#include <utility>

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

void Juki2D::Body2D::updateMotion(float dt, const std::vector<sf::FloatRect>& obstacles) {
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
void Juki2D::Body2D::setCollisionFilter(CollisionFilter filter) {
    collisionFilter = filter;
}

Juki2D::CollisionFilter Juki2D::Body2D::getCollisionFilter() const {
    return collisionFilter;
}

const std::vector<Juki2D::TriggerEvent>& Juki2D::Body2D::getTriggerEvents() const {
    return triggerEvents;
}

void Juki2D::Body2D::update(float dt, const std::vector<sf::FloatRect>& obstacles) {
    updateMotion(dt, obstacles);
    updateTriggers({});
}

void Juki2D::Body2D::updateColliders(
    float dt, const std::vector<Collider2D>& colliders
) {
    std::unordered_set<ColliderId> ids;
    std::vector<sf::FloatRect> solids;

    solids.reserve(colliders.size());

    for (const auto& collider : colliders) {
        if (collider.id == 0 || !ids.insert(collider.id).second) {
            throw std::invalid_argument("Collider IDs must be nonzero and unique.");
        }

        if (!collider.isTrigger && canInteract(collisionFilter, collider.filter)) {
            solids.push_back(collider.bounds);
        }
    }

    updateMotion(dt, solids);
    updateTriggers(colliders);
}

void Juki2D::Body2D::updateTriggers(const std::vector<Collider2D>& colliders) {
    std::vector<ColliderId> current;

    for (const auto& collider : colliders) {
        if (collider.isTrigger && canInteract(collisionFilter, collider.filter)
            && Collision::isColliding(bounds, collider.bounds)) {
            current.push_back(collider.id);
        }
    }

    std::sort(current.begin(), current.end());
    triggerEvents.clear();

    for (const auto id : current) {
        const bool wasInside =
            std::binary_search(activeTriggers.begin(), activeTriggers.end(), id);
        triggerEvents.push_back({id, wasInside ? TriggerPhase::Stay : TriggerPhase::Enter});
    }

    for (const auto id : activeTriggers) {
        if (!std::binary_search(current.begin(), current.end(), id)) {
            triggerEvents.push_back({id, TriggerPhase::Exit});
        }
    }

    activeTriggers = std::move(current);
}
