#pragma once
#include <SFML/Graphics.hpp>
#include "Player.hpp"

class Game {
private:
	sf::RenderWindow window;
	sf::RectangleShape platform;

	Player player;

	void processEvents();
	void update(float dt);
	void render();

public:
	Game();
	void run();
};