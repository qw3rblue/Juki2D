#pragma once
#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>

class Collision {
public:
	struct MoveResult {
		sf::Vector2f displacement{ 0.f, 0.f };
		std::vector<sf::Vector2f> normals;
	};

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

	static MoveResult moveSlide(
		sf::FloatRect bounds,
		sf::Vector2f displacement,
		const std::vector<sf::FloatRect>& obstacles
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