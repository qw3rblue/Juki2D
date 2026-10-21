#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"

class Game {
private:
	sf::RenderWindow window;
	sf::View camera;
	std::vector<sf::RectangleShape> platforms;

	Player player;

	void updateCamera();
	void processEvents();
	void update(float dt);
	void render();

public:
	Game();
	void run();
};