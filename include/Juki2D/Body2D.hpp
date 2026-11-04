#pragma once
#include <SFML/Graphics.hpp>
#include <Juki2D/Collider2D.hpp>
#include <vector>

namespace Juki2D {
    class Body2D {
    private:
        sf::FloatRect bounds;
        sf::Vector2f velocity = {0.f, 0.f};

        float gravity = 1200.f;
        bool grounded = false;

        CollisionFilter collisionFilter{};

        std::vector<ColliderId> activeTriggers;
        std::vector<TriggerEvent> triggerEvents;

        void updateMotion(float dt, const std::vector<sf::FloatRect>& obstacles);
        void updateTriggers(const std::vector<Collider2D>& colliders);

    public:
        sf::FloatRect getBounds() const;
        sf::Vector2f getVelocity() const;

        bool isGrounded() const;
        void setGravity(float gravity);
        void setVelocity(sf::Vector2f velocity);
        void setCollisionFilter(CollisionFilter filter);

        CollisionFilter getCollisionFilter() const;

        void update(float dt, const std::vector<sf::FloatRect>& obstacles);
        void updateColliders(float dt, const std::vector<Collider2D>& colliders);
        const std::vector<TriggerEvent>& getTriggerEvents() const;

        Body2D(sf::Vector2f position, sf::Vector2f size);
    };
}
