#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace Juki2D {
	class Body2D {
	private:
		sf::FloatRect bounds;
		sf::Vector2f velocity = { 0.f, 0.f };
		float gravity = 1200.f;
		bool grounded = false;

	public:
		sf::FloatRect getBounds() const;
		sf::Vector2f getVelocity() const;
		bool isGrounded() const;
		void setGravity(float gravity);
		void setVelocity(sf::Vector2f velocity);
		void update(float dt, const std::vector<sf::FloatRect>& obstacles);

		Body2D(sf::Vector2f position, sf::Vector2f size);
	};
}