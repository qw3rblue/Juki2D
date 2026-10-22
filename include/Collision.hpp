#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

class Collision {
public:
	struct SweepHit {
		float time;
		sf::Vector2f normal;
	};

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

	static std::optional<SweepHit> sweepAABB(
		const sf::FloatRect& moving,
		sf::Vector2f displacement,
		const sf::FloatRect& obstacle
	);
};